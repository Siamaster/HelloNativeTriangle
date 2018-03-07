#include "file.h"

#include "logger.h"

#include <cstring>
#include <fstream>
#include <sstream>

#include <stdlib.h>

static const char *kLogTag = "File";

namespace file {

    AAssetManager *asset_manager;

    string ReadFile(const char *path) {
        auto asset = AAssetManager_open(asset_manager, path, AASSET_MODE_UNKNOWN);
        if (asset == nullptr) {
            LOG_ERROR("Asset not found, path: %s", path);
            return "";
        }
        auto size = AAsset_getLength(asset);
        auto buffer = (char *) malloc(sizeof(char) * size);
        AAsset_read(asset, buffer, (size_t) (size));
        AAsset_close(asset);
        return buffer;
    }
}

