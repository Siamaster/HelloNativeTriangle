package com.siamaster.hellonativetriangle;

import android.content.res.AssetManager;

public class NativeAssetManager {

    static {
        System.loadLibrary("jni");
    }

    public native static void initialize(AssetManager assetManager);
}
