//
// Created by 86422 on 2024/2/29.
//

#include <jni.h>
#include <opencv2/opencv.hpp>
#include <android/bitmap.h>
#include <android/log.h>

#if HAVE_NEON
#include <arm_neon.h>
#endif

#define LOG_TAG "NDK_LOG"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

//using namespace cv;

void mFunARGB2Gray(AndroidBitmapInfo info, void* pixels)
{
    //cv::TickMeter tm1;
    //tm1.start();
    double t = (double)cv::getTickCount();

    uint32_t* pixel = nullptr;
    int a = 0, r = 0, g = 0, b = 0;
    int rows = info.height;
    int cols = info.width;

    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            pixel = (uint32_t*) pixels + info.width * y + x;
            a = (*pixel & 0xFF000000) >> 24;
            r = (*pixel & 0x00FF0000) >> 16;
            g = (*pixel & 0x0000FF00) >> 8;
            b = (*pixel & 0x000000FF) >> 0;
            int gray = (r * 38 + g * 75 + b * 15) >> 7;
            *pixel = ((a << 24) | (gray << 16) | (gray << 8) | gray);
        }
    }

    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
    //tm1.stop();
    LOGI("mFunARGB2Gray time: %lf", t);
}

void mOpenCVARGB2Gray(AndroidBitmapInfo info, void* pixels)
{
    //TickMeter tm2;
    //tm2.start();
    double t = (double)cv::getTickCount();

    cv::Mat mSrc(info.height, info.width, CV_8UC4, pixels);
    cv::Mat mGray, mRes;
    cv::cvtColor(mSrc, mGray, cv::COLOR_BGRA2GRAY);
    cv::cvtColor(mGray, mRes, cv::COLOR_GRAY2RGBA);
    memcpy(pixels, mRes.data, info.height * info.width * 4);

    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
    //tm2.stop();
    LOGI("mOpenCVARGB2Gray time: %lf", t);
}

void mNeonARGB2Gray(AndroidBitmapInfo info, void* pixels)
{
    //TickMeter tm3;
    //tm3.start();
    double t = (double)cv::getTickCount();

    unsigned short* dst = (unsigned short*)pixels;
    unsigned char*  src = (unsigned char*)pixels;
    uint8x8_t r = vdup_n_u8(38);
    uint8x8_t g = vdup_n_u8(75);
    uint8x8_t b = vdup_n_u8(15);
    uint16x8_t alp = vdupq_n_u16(255 << 8);

    uint16x8_t temp;
    uint8x8_t  gray;
    uint8x8x4_t argb;
    uint16x8_t high;
    uint16x8_t low;
    uint16x8x2_t res;

    int i, size = info.height * info.width / 8;
    for (i = 0; i < size; ++i)
    {
        argb = vld4_u8(src);
        temp = vmull_u8(argb.val[1], r);
        temp = vmlal_u8(temp, argb.val[2], g);
        temp = vmlal_u8(temp, argb.val[3], b);
        gray = vshrn_n_u16(temp, 7);
        src += 8 * 4;

        high = vorrq_u16(alp, vmovl_u8(gray));
        low = vorrq_u16(vshlq_n_u16(vmovl_u8(gray), 8), vmovl_u8((gray)));
        res = vzipq_u16(low, high);
        vst1q_u16(dst, res.val[0]);
        dst += 8;
        vst1q_u16(dst, res.val[0]);
        dst += 8;
    }

    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
    //tm3.stop();
    LOGI("mNeonARGB2Gray time: %lf", t);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_renderplayground_MainActivity_processImage(JNIEnv *env, jobject thiz,
                                                            jobject src) {

    AndroidBitmapInfo info;
    memset(&info, 0, sizeof(info));
    AndroidBitmap_getInfo(env, src, &info);
    if (info.width <= 0 || info.height <= 0 || (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888)) {
        return -1;
    }

    void* pixels = nullptr;
    int result = AndroidBitmap_lockPixels(env, src, &pixels);
    if (pixels == nullptr)
    {
        return -2;
    }

    mOpenCVARGB2Gray(info, pixels);

    mOpenCVARGB2Gray(info, pixels);

#if HAVE_NEON
    mNeonARGB2Gray(info, pixels);
#endif

    AndroidBitmap_unlockPixels(env, src);

    return 0;

}