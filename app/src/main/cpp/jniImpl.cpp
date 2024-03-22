
#include "jni.h"
#include <string>

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
Java_com_example_renderplayground_RenderNative_native_1OnSurfaceCreated(JNIEnv *env, jobject thiz,
                                                                        jobject assetManager) {
    //jobject is an invalid JNI transition frame reference or invalid reference
    // SIGABORT coredump
    // lifespan of assetManager!
    jobject globalRef = env->NewGlobalRef(assetManager);
    RenderContext::GetInstance()->OnSurfaceCreated(env, globalRef);
    //env->DeleteGlobalRef(globalRef);
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

/*JNIEXPORT void JNICALL
Java_com_example_renderplayground_MainActivity_setNativeAssetManager(JNIEnv *env, jobject thiz,
                                                                     jobject asset_manager) {
    mAssetManager = AAssetManager_fromJava(env, asset_manager);
}*/

#ifdef __cplusplus
}
#endif

