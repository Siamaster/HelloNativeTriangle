#pragma once

#include <android/asset_manager_jni.h>
#include <string>

using namespace std;

namespace file {
    extern AAssetManager *asset_manager;

    string ReadFile(const char *path);
}