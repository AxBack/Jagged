package com.wallpaper.axb.jagged;

import android.content.res.AssetManager;
import android.graphics.Rect;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

class JaggedRenderer implements GLSurfaceView.Renderer {

    private final AssetManager mAssetManager;
    private final NativeEngine mRenderEngine = new NativeEngine();

    private int mWidth;
    private int mHeight;

    private boolean mIsInitialized = false;

    public JaggedRenderer(AssetManager assetManager, Rect frame) {
        mAssetManager = assetManager;
        mWidth = frame.width();
        mHeight = frame.height();
    }

    public synchronized void destroy() {
        mRenderEngine.destroy();
    }

    @Override
    public synchronized void onSurfaceCreated(GL10 gl, EGLConfig config) {
        createEngine();
    }

    @Override
    public synchronized void onSurfaceChanged(GL10 gl, int width, int height) {
        mWidth = width;
        mHeight = height;
        createEngine();
    }

    @Override
    public synchronized void onDrawFrame(GL10 gl) {
        if (!mIsInitialized) {
            createEngine();
        }

        mRenderEngine.render();
    }

    private void createEngine() {
        if(mWidth == 0 || mHeight == 0)
            return;

        mRenderEngine.create(mAssetManager, mWidth, mHeight);
        mIsInitialized = true;
    }
}
