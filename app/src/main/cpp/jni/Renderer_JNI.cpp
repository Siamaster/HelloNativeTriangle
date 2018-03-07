#include <jni.h>

#include "game_loop.h"

#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <memory>

#define JNI_FUNCTION(return_type, method_name) extern "C" JNIEXPORT return_type JNICALL Java_com_siamaster_hellonativetriangle_Renderer_##method_name
#define THIS ((GameLoop *)env->GetLongField(obj, native_handle))
#define UNIQUE_PTR_WINDOW(env, surface) unique_ptr<ANativeWindow, function<void(ANativeWindow *)>>(ANativeWindow_fromSurface(env, surface), ANativeWindow_release)
#define UNIQUE_PTR_RENDERER(env, surface) make_unique<Renderer>(UNIQUE_PTR_WINDOW(env, surface))

static jfieldID native_handle;

JNI_FUNCTION(void, classInit)(JNIEnv *env, jclass clazz) {
    native_handle = env->GetFieldID(clazz, "mNativeHandle", "J");
}

JNI_FUNCTION(void, destroy)(JNIEnv *env, jobject obj) {
    delete THIS;
}

JNI_FUNCTION(jlong, create)(JNIEnv *env, jobject, jobject surface) {
    auto *self = new GameLoop(UNIQUE_PTR_RENDERER(env, surface));
    return jlong(self);
}

JNI_FUNCTION(void, changeSurface)(JNIEnv *env, jobject obj, jobject surface) {
    THIS->ChangeRenderer(UNIQUE_PTR_RENDERER(env, surface));
}

JNI_FUNCTION(void, start)(JNIEnv *env, jobject obj) {
    THIS->Start();
}