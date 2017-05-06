package com.wallpaper.axb.jagged;

import android.content.res.AssetManager;

class NativeEngine {

    public native boolean create(AssetManager assetManager, int width, int height);
    public native void destroy();
    public native boolean render();
    public native void resume();
    public native void pause();
    public native void onTouch(float x, float y);


    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-engine-lib");
    }
}
