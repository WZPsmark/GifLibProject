
#ifndef GIFLIBPROJECT_LOG_H
#define GIFLIBPROJECT_LOG_H
#define LOG_DEBUG true;
#define TAG "smark"

#include <android/log.h>

#ifdef LOG_DEBUG
#define LOGI(...)\
__android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
#else
#define LOGI(...)
#endif
#endif