
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
Java_com_example_renderplayground_RenderNative_native_1OnSurfaceCreated(JNIEnv *env, jobject thiz)
{
    // TODO: implement native_OnSurfaceCreated()
    RenderContext::GetInstance()->OnSurfaceCreated();
}

JNIEXPORT void JNICALL
Java_com_example_renderplayground_RenderNative_native_1OnSurfaceChanged(JNIEnv *env, jobject thiz, jint width, jint height)
{
    // TODO: implement native_OnSurfaceChanged()
    RenderContext::GetInstance()->OnSurfaceChanged(width, height);
}

JNIEXPORT void JNICALL
Java_com_example_renderplayground_RenderNative_native_1OnDrawFrame(JNIEnv *env, jobject thiz)
{
    // TODO: implement native_OnDrawFrame()
    RenderContext::GetInstance()->OnDrawFrame();
}

JNIEXPORT void JNICALL
Java_com_example_renderplayground_RenderNative_native_1Init(JNIEnv *env, jobject thiz)
{
    // TODO: implement native_Init()
    RenderContext::GetInstance();
}

JNIEXPORT void JNICALL
Java_com_example_renderplayground_RenderNative_native_1UnInit(JNIEnv *env, jobject thiz)
{
    // TODO: implement native_UnInit()
    RenderContext::DestroyInstance();
}

JNIEXPORT void JNICALL
Java_com_example_renderplayground_RenderNative_native_1UpdateTransformMatrix(JNIEnv *env,
                                                                             jobject thiz,
                                                                             jfloat rotate_x,
                                                                             jfloat rotate_y,
                                                                             jfloat scale_x,
                                                                             jfloat scale_y)
                                                                             {
    RenderContext::GetInstance()->UpdateTransformMatrix(rotate_x, rotate_y, scale_x, scale_y);
    // TODO: implement native_UpdateTransformMatrix()
}

JNIEXPORT void JNICALL
Java_com_example_renderplayground_RenderNative_native_1SetImageData(JNIEnv *env, jobject thiz,
                                                                    jint format, jint width,
                                                                    jint height, jbyteArray imageData)
                                                                    {
                                                                        int len = env->GetArrayLength (imageData);
                                                                        uint8_t* buf = new uint8_t[len];
                                                                        env->GetByteArrayRegion(imageData, 0, len, reinterpret_cast<jbyte*>(buf));
                                                                        RenderContext::GetInstance()->SetImageData(format, width, height, buf);
                                                                        delete[] buf;
                                                                        env->DeleteLocalRef(imageData);
    // TODO: implement native_SetImageData()
}

#ifdef __cplusplus
}
#endif
