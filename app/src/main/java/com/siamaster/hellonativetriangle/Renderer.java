package com.siamaster.hellonativetriangle;

import android.view.Surface;
import android.view.SurfaceHolder;

public class Renderer implements SurfaceHolder.Callback {

    private long mNativeHandle; // Accessed from JNI
    private SurfaceHolder mSurfaceHolder;

    static {
        System.loadLibrary("jni");
        classInit();
    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {
        mSurfaceHolder = surfaceHolder;
        mNativeHandle = create(surfaceHolder.getSurface());
        nativeStart();
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int format, int width, int height) {
        if (surfaceHolder != mSurfaceHolder) {
            mSurfaceHolder = surfaceHolder;
            changeSurface(surfaceHolder.getSurface());
        }
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
        destroy();
        mNativeHandle = 0;
    }

    private native void destroy();
    private native long create(Surface surface);
    private native void changeSurface(Surface surface);
    private native void nativeStart();
    private native static void classInit();
}
