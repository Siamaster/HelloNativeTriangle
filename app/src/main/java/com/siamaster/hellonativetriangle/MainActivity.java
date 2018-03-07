package com.siamaster.hellonativetriangle;

import android.app.Activity;
import android.os.Bundle;
import android.view.SurfaceView;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        NativeAssetManager.initialize(getAssets());
        SurfaceView surfaceView = findViewById(R.id.surfaceview);
        surfaceView.getHolder().addCallback(new Renderer());
    }
}
