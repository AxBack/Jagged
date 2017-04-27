package com.wallpaper.axb.jagged;

/**
 * Created by AxB on 4/22/2017.
 */

public class NativeEngine {

    public native boolean create(int width, int height);
    public native void destroy();
    public native boolean render();


    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-engine-lib");
    }
}
