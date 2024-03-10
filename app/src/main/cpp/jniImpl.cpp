
#include "jni.h"
#include <string>

// set affinity ---------------------
#include <unistd.h>
#include <pthread.h>

#include <sched.h>
#include "setAffinity/setAffinity.cpp"
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
    int coreNum = 0; //getCoreNums();
    int cpuIndex = 0; //getMaxFreqIndex(coreNum);

    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(cpuIndex, &mask);
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1)
    {
        return -1;
    }
    return 0;
}