#pragma once

#include <android/native_window.h>
#include <EGL/egl.h> // must come after native include
#include <GLES/gl.h>
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

    GLuint shader_program_;
    GLuint vertex_array_;
};
