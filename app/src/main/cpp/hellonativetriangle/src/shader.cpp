#include "logger.h"

#include <GLES3/gl3.h>
#include <tuple>

static const char *kLogTag = "Shader";

using namespace std;

namespace shader {

    static const int info_buffer_size = 512;

    tuple<GLuint, bool> CompileShader(const GLchar *source, GLenum shader_type) {
        auto shader = glCreateShader(shader_type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char info[info_buffer_size];
            glGetShaderInfoLog(shader, info_buffer_size, nullptr, info);
            LOG_ERROR("Shader error: %s", info);
        }
        return make_tuple(shader, success != 0);
    }

    tuple<GLuint, bool> LinkProgram(GLuint vertex_shader, GLuint fragment_shader) {
        auto program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            char info[info_buffer_size];
            glGetShaderInfoLog(program, info_buffer_size, nullptr, info);
            LOG_ERROR("Shader error: %s", info);
        }
        return make_tuple(program, success != 0);
    }

    tuple<GLuint, bool> LoadShader(const char *vertex_source, const char *fragment_source) {
        bool success;
        GLuint vertex_shader;
        tie(vertex_shader, success) = CompileShader(vertex_source, GL_VERTEX_SHADER);
        if (!success) {
            LOG_ERROR("Failed to compile vertex shader");
            return make_tuple((GLuint) 0, false);
        }

        GLuint fragment_shader;
        tie(fragment_shader, success) = CompileShader(fragment_source, GL_FRAGMENT_SHADER);
        if (!success) {
            LOG_ERROR("Failed to compile fragment shader");
            glDeleteShader(vertex_shader);
            return make_tuple((GLuint) 0, false);
        }

        GLuint program;
        tie(program, success) = LinkProgram(vertex_shader, fragment_shader);
        if (!success) {
            LOG_ERROR("Failed to link program");
        }

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return make_tuple(program, success);
    }
}
