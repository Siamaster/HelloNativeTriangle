#include "file.h"

namespace file {

    AAssetManager *asset_manager;

    vector<char> Read(const char *path) {
        AAsset *asset = AAssetManager_open(asset_manager, path, AASSET_MODE_STREAMING);

        off64_t length = AAsset_getLength64(asset);
        vector<char> vector;
        vector.reserve(static_cast<uint64_t>(length));

        size_t remaining_bytes = static_cast<size_t>(AAsset_getRemainingLength64(asset));
        while (remaining_bytes != 0) {
            size_t chunk_size_limit = 1000 * 1024;  // 1 Mb is the maximum chunk size for compressed assets
            size_t chunk_size = remaining_bytes >= chunk_size_limit ? chunk_size_limit : remaining_bytes;
            char chunk[chunk_size];

            if (AAsset_read(asset, chunk, remaining_bytes) > 0) {
                vector.insert(vector.end(), chunk, chunk + remaining_bytes);
                remaining_bytes = static_cast<size_t>(AAsset_getRemainingLength64(asset));
            }
        }

        AAsset_close(asset);

        return vector;
    }
}

