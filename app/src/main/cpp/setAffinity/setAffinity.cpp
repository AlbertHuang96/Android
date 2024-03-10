//
// Created by 86422 on 2024/3/10.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

/*int getCoreNums()
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
}*/

/*int getMaxFreqIndex(int coreNum)
{
    int maxFreq = -1;
    int index = -1;
    try
    {
        *//*for (int i = 0; i < coreNum; i++)
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
        }*//*
    }
    catch (const std::exception& e)
    {
        //e.what()
    }
    return index;
}*/
