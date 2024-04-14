
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
//#include <cpuinfo/cpuinfo.h>

//----------------------------------

#include "art_methods.h"
#include "UtilLog.h"

#include "RenderContext/RenderContext.h"

#ifdef __cplusplus
extern "C" {
#endif


/*JNIEXPORT jstring JNICALL
Java_com_example_renderplayground_MainActivity_native_1Helloworld(JNIEnv *env, jobject thiz) {

    return env->NewStringUTF("Hello world");
}*/


JNIEXPORT void JNICALL
Java_com_example_renderplayground_RenderNative_native_1OnSurfaceCreated(JNIEnv *env, jobject thiz) {

    RenderContext::GetInstance()->OnSurfaceCreated();
}

JNIEXPORT void JNICALL
Java_com_example_renderplayground_RenderNative_native_1OnSurfaceChanged(JNIEnv *env, jobject thiz,
                                                                        jint width, jint height) {

    RenderContext::GetInstance()->OnSurfaceChanged(width, height);
}

JNIEXPORT void JNICALL
Java_com_example_renderplayground_RenderNative_native_1OnDrawFrame(JNIEnv *env, jobject thiz) {

    RenderContext::GetInstance()->OnDrawFrame();
}

JNIEXPORT void JNICALL
Java_com_example_renderplayground_RenderNative_native_1Init(JNIEnv *env, jobject thiz) {

    RenderContext::GetInstance();
}

JNIEXPORT void JNICALL
Java_com_example_renderplayground_RenderNative_native_1UnInit(JNIEnv *env, jobject thiz) {

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


/*void getCacheInfo(const char* name, const struct cpuinfo_cache* cache, std::ostringstream& oss)
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
}*/

/*extern "C"
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
}*/

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

//https://android.googlesource.com/platform/art/+/refs/tags/android-platform-12.0.0_r30/runtime/art_method.h
extern "C"
JNIEXPORT void JNICALL
Java_com_example_renderplayground_FixDexNative_replace(JNIEnv *env, jclass clazz,
                                                       jobject wrong_method, jobject right_method, jclass clazz2) {

    LOGCATE("size of std::atomic<std::uint32_t> = %ld", sizeof(std::atomic<std::uint32_t>));
    ///RangerEngine: size of std::atomic<std::uint32_t> = 4
    art::ArtMethod* wrong = (art::ArtMethod*)env->FromReflectedMethod(wrong_method);
    art::ArtMethod* right = (art::ArtMethod*)env->FromReflectedMethod(right_method);
    LOGCATE("size of art::ArtMethod = %ld", sizeof(art::ArtMethod));
    //RangerEngine: size of art::ArtMethod = 32
    // FromReflectedMethod start address of the ArtMethod of the corresponding method
    // direct or virtual method

    if (wrong == nullptr || right == nullptr)
    {
        LOGCATE("replace null ptr");
        return;
    }

    // jobject jclass
    size_t firstMethod = (size_t)env->GetStaticMethodID(clazz2, "f1", "()V");
    size_t secondMethod = (size_t)env->GetStaticMethodID(clazz2, "f2", "()V");
    size_t methodSize = secondMethod - firstMethod;
    LOGCATE("size of ArtMethod = %ld", methodSize);//E/RangerEngine: size of ArtMethod = 2
    //memcpy(wrong, right, methodSize);

    //wrong->declaring_class_ = right->declaring_class_;
    //wrong->access_flags_ = right->access_flags_;
    //wrong->access_flags_.exchange(right->access_flags_);

    //wrong->dex_method_index_ = right->dex_method_index_;
    //wrong->method_index_ = right->method_index_;

    //wrong->hotness_count_ = right->hotness_count_;
    //wrong->imt_index_ = right->imt_index_;

    //wrong->ptr_sized_fields_.data_ = right->ptr_sized_fields_.data_;
    //wrong->ptr_sized_fields_.entry_point_from_quick_compiled_code_ = right->ptr_sized_fields_.entry_point_from_quick_compiled_code_;

}