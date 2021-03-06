#pragma once

#include <android/native_window.h>
#include <EGL/egl.h> // must come after native include
#include <GLES3/gl3.h>
#include <glm/glm.hpp>

#include <functional>
#include <memory>

using namespace std;

class Renderer {
public:
    explicit Renderer(unique_ptr<ANativeWindow, function<void(ANativeWindow *)>> window)
            : window_(move(window)) {}

    ~Renderer();
    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;

    void Initialize();

    void DrawFrame();
    void Destroy();

private:

    unique_ptr<ANativeWindow, function<void(ANativeWindow *)>> window_;

    EGLDisplay display_ = nullptr;
    EGLSurface surface_ = nullptr;
    EGLContext context_ = nullptr;

    EGLint surface_width_ = 0;
    EGLint surface_height_ = 0;

    GLuint texture_ = 0;
    GLuint shader_program_ = 0;
    GLuint vertex_array_ = 0;

    glm::mat4 model_{};
};
