#pragma once

#include <GLES3/gl3.h>

#include <vector>

using namespace std;

namespace texture {
    GLuint LoadPNG(const vector<char> &data);
}