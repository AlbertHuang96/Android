//
// Created by 86422 on 2024/2/1.
//

#ifndef RENDERPLAYGROUND_UTILLOG_H
#define RENDERPLAYGROUND_UTILLOG_H

#include<android/log.h>
#include <sys/time.h>

#define  LOG_TAG "RangerEngine"

#define  LOGCATE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGCATV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__)
#define  LOGCATD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGCATI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

#define GO_CHECK_GL_ERROR(...)   LOGCATE("CHECK_GL_ERROR %s glGetError = %d, line = %d, ",  __FUNCTION__, glGetError(), __LINE__)

#endif //RENDERPLAYGROUND_UTILLOG_H
