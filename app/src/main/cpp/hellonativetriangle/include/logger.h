#pragma once

#include <android/log.h>

#define LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, kLogTag, __VA_ARGS__)