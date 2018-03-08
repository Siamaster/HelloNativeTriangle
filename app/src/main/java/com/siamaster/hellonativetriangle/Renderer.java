package com.siamaster.hellonativetriangle;

import android.view.Surface;
import android.view.SurfaceHolder;

public class Renderer implements SurfaceHolder.Callback {

    static {
        System.loadLibrary("jni");
        classInit();
    }

    private long mNativeHandle; // Accessed from JNI

    public void resume() {
        if (mNativeHandle != 0) {
            nativeResume();
        }
    }

    public void pause() {
        if (mNativeHandle != 0) {
            nativePause();
        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {
        mNativeHandle = create(surfaceHolder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int format, int width, int height) {
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
        destroy();
        mNativeHandle = 0;
    }

    private native void destroy();
    private native long create(Surface surface);
    private native void changeSurface(Surface surface);
    private native void nativeResume();
    private native void nativePause();
    private native static void classInit();
}
