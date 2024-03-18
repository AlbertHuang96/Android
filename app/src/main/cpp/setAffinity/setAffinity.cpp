//
// Created by 86422 on 2024/3/10.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <thread>
#include "UtilLog.h"

int getCoreNums()
{
    std::ifstream cpuinfo("/proc/cpuinfo");
    int cores = 0;
    std::string line;
    while (std::getline(cpuinfo, line))
    {
        if (line.substr(0, 9) == "processor")
        {
            cores++;
        }
    }
    cpuinfo.close();
    return cores;
}

int getMaxFreqIndex(int coreNum)
{
    int maxFreq = -1;
    int index = -1;
    try
    {
        for (int i = 0; i < coreNum; i++)
        {
            std::string filename = "/sys/devices/system/cpu/cpu" + std::to_string(i) + "/cpufreq/cpuinfo_max_freq";
            std::ifstream cpufile(filename);
            if (cpufile.good())
            {
                std::string line;
                std::getline(cpufile, line);
                std::stringstream ss(line);
                int freqBound;
                ss >> freqBound;
                if (freqBound > maxFreq)
                {
                    maxFreq = freqBound;
                    index = i;
                }
                cpufile.close();
            }
        }
    }
    catch (const std::exception& e)
    {
        //e.what()
    }
    return index;
}

int setAffinity(int core)
{
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(core, &mask);
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1)
    {
        return -1;
    }
    return 0;
}

constexpr bool FALSE_SHARING = true;
constexpr size_t CACHE_LINE_SIZE = 64, SECOND_ALIGN_SIZE = FALSE_SHARING ? sizeof(int) : CACHE_LINE_SIZE;

constexpr int A_THREAD_CORE = 0, B_THREAD_CORE = 3;
constexpr size_t CPU_NUM = 8;

// core
// 0 3
//RangerEngine: tag thread-a spent 22370965616 ns
//RangerEngine: tag thread-b spent 22928803064 ns

// 0 1
//RangerEngine: tag thread-b spent 10913725934 ns
//RangerEngine: tag thread-a spent 10920305881 ns

// 0 0
//RangerEngine: tag thread-a spent 14890188953 ns
//RangerEngine: tag thread-b spent 14886654838 ns

struct AlignStruct
{
    std::atomic<int> a alignas(CACHE_LINE_SIZE);
    std::atomic<int> b alignas(SECOND_ALIGN_SIZE);
};

void testFalseSharing()
{
    AlignStruct alignStruct;
    LOGCATE("size of a = %d", sizeof(std::atomic<int>));
    LOGCATE("size of b = %d", sizeof(alignStruct.b));
    auto runnable = [](std::atomic<int>* val, int core, const char* tag)
    {
        setAffinity(core);
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        for (size_t r = 100000000; r>0;r--)
        {
            ++*val;
        }
        std::chrono::steady_clock::time_point last = std::chrono::steady_clock::now();
        auto dt = last - start;

        LOGCATE("tag %s spent %lld ns", tag, dt.count());
    };
    std::thread threadA(runnable, &alignStruct.a, A_THREAD_CORE, "thread-a");
    std::thread threadB(runnable, &alignStruct.b, B_THREAD_CORE, "thread-b");
    threadA.join();
    threadB.join();
}
