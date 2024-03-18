
#include "jni.h"
#include <string>

// set affinity ---------------------
#include <unistd.h>
//#include <pthread.h>

#include <sched.h>
#include "setAffinity/setAffinity.h"
//----------------------------------
//-----cpu cache info-------------

#include <sstream>
#include <cpuinfo/cpuinfo.h>

//----------------------------------

#include "RenderContext/RenderContext.h"

#ifdef __cplusplus
extern "C" {
#endif


/*JNIEXPORT jstring JNICALL
Java_com_example_renderplayground_MainActivity_native_1Helloworld(JNIEnv *env, jobject thiz) {
    // TODO: implement native_Helloworld()
    return env->NewStringUTF("Hello world");
}*/


JNIEXPORT void JNICALL
Java_com_example_renderplayground_RenderNative_native_1OnSurfaceCreated(JNIEnv *env, jobject thiz) {
    // TODO: implement native_OnSurfaceCreated()
    RenderContext::GetInstance()->OnSurfaceCreated();
}

JNIEXPORT void JNICALL
Java_com_example_renderplayground_RenderNative_native_1OnSurfaceChanged(JNIEnv *env, jobject thiz,
                                                                        jint width, jint height) {
    // TODO: implement native_OnSurfaceChanged()
    RenderContext::GetInstance()->OnSurfaceChanged(width, height);
}

JNIEXPORT void JNICALL
Java_com_example_renderplayground_RenderNative_native_1OnDrawFrame(JNIEnv *env, jobject thiz) {
    // TODO: implement native_OnDrawFrame()
    RenderContext::GetInstance()->OnDrawFrame();
}

JNIEXPORT void JNICALL
Java_com_example_renderplayground_RenderNative_native_1Init(JNIEnv *env, jobject thiz) {
    // TODO: implement native_Init()
    RenderContext::GetInstance();
}

JNIEXPORT void JNICALL
Java_com_example_renderplayground_RenderNative_native_1UnInit(JNIEnv *env, jobject thiz) {
    // TODO: implement native_UnInit()
    RenderContext::DestroyInstance();
}

JNIEXPORT void JNICALL
Java_com_example_renderplayground_RenderNative_native_1UpdateTransformMatrix(JNIEnv *env,
                                                                             jobject thiz,
                                                                             jfloat rotate_x,
                                                                             jfloat rotate_y,
                                                                             jfloat scale_x,
                                                                             jfloat scale_y) {
    RenderContext::GetInstance()->UpdateTransformMatrix(rotate_x, rotate_y, scale_x, scale_y);
    // TODO: implement native_UpdateTransformMatrix()
}

#ifdef __cplusplus
}
#endif


int getCores()
{
    int cores = sysconf(_SC_NPROCESSORS_CONF);
    return cores;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_renderplayground_MainActivity_bindThreadToCore(JNIEnv *env, jobject thiz,
                                                                jint core) {
    int num = getCores();
    if (core >= num)
    {
        return -1;
    }

    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(core, &mask);
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1)
    {
        return -1;
    }
    return 0;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_renderplayground_MainActivity_bindPidToCore(JNIEnv *env, jobject thiz, jint pid,
                                                             jint core) {
    int num = getCores();
    if (core >= num)
    {
        return -1;
    }

    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(core, &mask);
    if (sched_setaffinity(pid, sizeof(mask), &mask) == -1)
    {
        return -1;
    }
    return 0;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_renderplayground_MainActivity_bindThreadToCore2(JNIEnv *env, jobject thiz,
                                                                 jint core) {
    int coreNum = getCoreNums();
    int cpuIndex = getMaxFreqIndex(coreNum);

    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(cpuIndex, &mask);
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1)
    {
        return -1;
    }
    return 0;
}


void getCacheInfo(const char* name, const struct cpuinfo_cache* cache, std::ostringstream& oss)
{
    oss << "CPU Cache: " << name << std::endl;
    oss << " > size            : " << cache->size << std::endl;
    oss << " > associativity   : " << cache->associativity   << std::endl;
    oss << " > sets            : " << cache->sets            << std::endl;
    oss << " > partitions      : " << cache->partitions      << std::endl;
    oss << " > line_size       : " << cache->line_size       << std::endl;
    oss << " > flags           : " << cache->flags           << std::endl;
    oss << " > processor_start : " << cache->processor_start << std::endl;
    oss << " > processor_count : " << cache->processor_count << std::endl;
    oss << std::endl;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_renderplayground_MainActivity_getCpuInfo(JNIEnv *env, jobject thiz) {

    cpuinfo_initialize();
    const struct cpuinfo_processor* proc = cpuinfo_get_current_processor();

    std::ostringstream oss;

    if (proc->cache.l1d)
    {
        getCacheInfo("L1 Data", proc->cache.l1d, oss);
    }

    if (proc->cache.l1i)
    {
        getCacheInfo("L1 Instruction", proc->cache.l1i, oss);
    }

    if (proc->cache.l2)
    {
        getCacheInfo("L2", proc->cache.l2, oss);
    }

    if (proc->cache.l3)
    {
        getCacheInfo("L3", proc->cache.l3, oss);
    }

    if (proc->cache.l4)
    {
        getCacheInfo("L4", proc->cache.l4, oss);
    }

    std::string ossString = oss.str();
    const char* ossChar = ossString.c_str();
    jstring jstrData = (env)->NewStringUTF( ossChar);
    return jstrData;
}

/*constexpr bool FALSE_SHARING = true;
constexpr size_t CACHE_LINE_SIZE = 64, SECOND_ALIGN_SIZE = FALSE_SHARING ? sizeof(int) : CACHE_LINE_SIZE;

constexpr int A_THREAD_CORE = 0, B_THREAD_CORE = 1;
constexpr size_t CPU_NUM = 8;*/

/*struct AlignStruct
{
    std::atomic<int> a alignas(CACHE_LINE_SIZE);
    std::atomic<int> b alignas(SECOND_ALIGN_SIZE);
};*/

extern "C"
JNIEXPORT void JNICALL
Java_com_example_renderplayground_MainActivity_testFalseSharing(JNIEnv *env, jobject thiz) {
    testFalseSharing();
}