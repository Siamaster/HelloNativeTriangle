#pragma once

#include <android/asset_manager_jni.h>

#include <vector>

using namespace std;

namespace file {
    extern AAssetManager *asset_manager;

    vector<char> Read(const char *path);
}