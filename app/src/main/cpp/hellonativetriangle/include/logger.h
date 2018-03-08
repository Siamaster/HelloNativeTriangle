#pragma once

#include <android/log.h>

#define LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, "HelloNativeTriangle", __VA_ARGS__)