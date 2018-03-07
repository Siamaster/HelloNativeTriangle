#include "renderer.h"

#include "file.h"
#include "logger.h"
#include "shader.h"

#include <GLES3/gl3.h>

const char *kLogTag = "RendererBase";

using namespace std;

Renderer::~Renderer() {
    Destroy();
}

void Renderer::Initialize() {

    // Initialize EGL

    display_ = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    if (display_ == EGL_NO_DISPLAY) {
        LOG_ERROR("eglGetDisplay() returned error %d", eglGetError());
        return;
    }
    if (!eglInitialize(display_, nullptr, nullptr)) {
        LOG_ERROR("eglInitialize() returned error %d", eglGetError());
        return;
    }
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };

    EGLConfig config;
    EGLint numConfigs;

    if (!eglChooseConfig(display_, attribs, &config, 1, &numConfigs)) {
        LOG_ERROR("eglChooseConfig() returned error %d", eglGetError());
        Destroy();
        return;
    }

    EGLint format;

    if (!eglGetConfigAttrib(display_, config, EGL_NATIVE_VISUAL_ID, &format)) {
        LOG_ERROR("eglGetConfigAttrib() returned error %d", eglGetError());
        Destroy();
        return;
    }

    eglChooseConfig(display_, attribs, &config, 1, &numConfigs);
    const EGLint context_attribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE};

    context_ = eglCreateContext(display_, config, EGL_NO_CONTEXT, context_attribs);

    if (!context_) {
        LOG_ERROR("eglCreateContext() returned error %d", eglGetError());
        Destroy();
        return;
    }
    surface_ = eglCreateWindowSurface(display_, config, window_.get(), nullptr);

    if (!surface_) {
        LOG_ERROR("eglCreateWindowSurface() returned error %d", eglGetError());
        Destroy();
        return;
    }

    if (!eglMakeCurrent(display_, surface_, surface_, context_)) {
        LOG_ERROR("eglMakeCurrent() returned error %d", eglGetError());
        Destroy();
        return;
    }

    // Initialize shader

    bool shader_compiled;
    tie(shader_program_, shader_compiled) = shader::LoadShader(
            file::ReadFile("shaders/vertex_shader.glsl").c_str(),
            file::ReadFile("shaders/fragment_shader.glsl").c_str());

    if (!shader_compiled) {
        LOG_ERROR("Couldn't create shader program");
        Destroy();
        return;
    }

    glUseProgram(shader_program_);

    // Initialize vertex array

    GLfloat vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
    };

    GLuint buffer;
    glGenVertexArrays(1, &vertex_array_);
    glGenBuffers(1, &buffer);
    glBindVertexArray(vertex_array_);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
}

void Renderer::DrawFrame() {
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    if (!eglSwapBuffers(display_, surface_)) {
        LOG_ERROR("eglSwapBuffers() returned error %d", eglGetError());
    }
}

void Renderer::Destroy() {
    eglMakeCurrent(display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(display_, context_);
    eglDestroySurface(display_, surface_);
    eglTerminate(display_);

    display_ = EGL_NO_DISPLAY;
    surface_ = EGL_NO_SURFACE;
    context_ = EGL_NO_CONTEXT;
}
