#include "renderer.h"

#include "file.h"
#include "logger.h"
#include "shader.h"

#include <GLES3/gl3.h>
#include <glm/ext.hpp>

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
    if (eglInitialize(display_, nullptr, nullptr) == EGL_FALSE) {
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

    if (eglChooseConfig(display_, attribs, &config, 1, &numConfigs) == EGL_FALSE) {
        LOG_ERROR("eglChooseConfig() returned error %d", eglGetError());
        Destroy();
        return;
    }

    EGLint format;

    if (eglGetConfigAttrib(display_, config, EGL_NATIVE_VISUAL_ID, &format) == EGL_FALSE) {
        LOG_ERROR("eglGetConfigAttrib() returned error %d", eglGetError());
        Destroy();
        return;
    }

    eglChooseConfig(display_, attribs, &config, 1, &numConfigs);
    const EGLint context_attribs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};

    context_ = eglCreateContext(display_, config, EGL_NO_CONTEXT, context_attribs);

    if (context_ == nullptr) {
        LOG_ERROR("eglCreateContext() returned error %d", eglGetError());
        Destroy();
        return;
    }
    surface_ = eglCreateWindowSurface(display_, config, window_.get(), nullptr);

    if (surface_ == nullptr) {
        LOG_ERROR("eglCreateWindowSurface() returned error %d", eglGetError());
        Destroy();
        return;
    }

    if (eglMakeCurrent(display_, surface_, surface_, context_) == EGL_FALSE) {
        LOG_ERROR("eglMakeCurrent() returned error %d", eglGetError());
        Destroy();
        return;
    }

    if (!eglQuerySurface(display_, surface_, EGL_WIDTH, &surface_width_) ||
        !eglQuerySurface(display_, surface_, EGL_HEIGHT, &surface_height_)) {
        LOG_ERROR("eglQuerySurface() returned error %d", eglGetError());
        Destroy();
        return;
    }

    // Initialize matrices

    float ratio = (float) glm::min(surface_width_, surface_height_) / glm::max(surface_width_, surface_height_);
    if (surface_width_ < surface_height_) {
        model_ = glm::scale(model_, glm::vec3(1.0f, ratio, 1.0f));
    } else {
        model_ = glm::scale(model_, glm::vec3(ratio, 1.0f, 1.0f));
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
            // Position   Color
            -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f, 1.0f
    };

    GLuint buffer;
    glGenVertexArrays(1, &vertex_array_);
    glGenBuffers(1, &buffer);
    glBindVertexArray(vertex_array_);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
}

void Renderer::DrawFrame() {
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    model_ = glm::rotate(model_, 0.01f, glm::vec3(0, 0, 1));
    glUniformMatrix4fv(glGetUniformLocation(shader_program_, "uModel"), 1, GL_FALSE, glm::value_ptr(model_));

    glDrawArrays(GL_TRIANGLES, 0, 3);

    if (eglSwapBuffers(display_, surface_) == EGL_FALSE) {
        LOG_ERROR("eglSwapBuffers() returned error %d", eglGetError());
    }
}

void Renderer::Destroy() {
    if (display_ == EGL_NO_DISPLAY) {
        return;
    }
    eglMakeCurrent(display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(display_, context_);
    eglDestroySurface(display_, surface_);
    eglTerminate(display_);

    display_ = EGL_NO_DISPLAY;
    surface_ = EGL_NO_SURFACE;
    context_ = EGL_NO_CONTEXT;
}
