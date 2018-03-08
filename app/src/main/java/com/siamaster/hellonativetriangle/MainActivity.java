package com.siamaster.hellonativetriangle;

import android.app.Activity;
import android.os.Bundle;
import android.view.SurfaceView;

public class MainActivity extends Activity {

    private Renderer mRenderer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        NativeAssetManager.initialize(getAssets());
        mRenderer = new Renderer();
        SurfaceView surfaceView = findViewById(R.id.surfaceview);
        surfaceView.getHolder().addCallback(mRenderer);
    }

    @Override
    protected void onResume() {
        super.onResume();
        mRenderer.resume();
    }

    @Override
    protected void onPause() {
        mRenderer.pause();
        super.onPause();
    }
}
