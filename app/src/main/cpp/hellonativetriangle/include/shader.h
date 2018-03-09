#pragma once

#include <GLES3/gl3.h>

#include <vector>

using namespace std;

namespace shader {
    GLuint Load(const vector<char> &vertex_source,const vector<char> &fragment_source);
}