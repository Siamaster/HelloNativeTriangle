#pragma once

#include <GLES3/gl3.h>
#include <tuple>

using namespace std;

namespace shader {
    tuple<GLuint, bool> LoadShader(const char *vertex_source, const char *fragment_source);
}