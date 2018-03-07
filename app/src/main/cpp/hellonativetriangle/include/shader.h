#pragma once

using namespace std;

namespace shader {
    tuple<GLuint, bool> LoadShader(const char *vertex_source, const char *fragment_source);
}