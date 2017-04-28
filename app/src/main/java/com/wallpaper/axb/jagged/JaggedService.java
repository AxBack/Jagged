package com.wallpaper.axb.jagged;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.service.wallpaper.WallpaperService;
import android.view.SurfaceHolder;

public class JaggedService extends WallpaperService {

    private JaggedEngine mEngine;

    @Override
    public void onCreate() {
        super.onCreate();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }

    @Override
    public Engine onCreateEngine() {
        mEngine = new JaggedEngine();
        return mEngine;
    }

    private class JaggedEngine extends WallpaperService.Engine {

        private JaggedSurfaceView mSurfaceView;
        private JaggedRenderer mRenderer;

        @Override
        public void onCreate(SurfaceHolder surfaceHolder) {
            super.onCreate(surfaceHolder);

            mSurfaceView = new JaggedSurfaceView(JaggedService.this);
            mSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 1);
            mSurfaceView.setEGLContextClientVersion(3);
            mSurfaceView.setPreserveEGLContextOnPause(true);

            mRenderer = new JaggedRenderer(getAssets(), mSurfaceView.getHolder().getSurfaceFrame());
            mSurfaceView.setRenderer(mRenderer);
            mSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
        }

        @Override
        public void onDestroy() {
            super.onDestroy();
            mSurfaceView.onDestroy();
            mRenderer.destroy();

            mSurfaceView = null;
            mRenderer = null;
        }

        @Override
        public void onSurfaceRedrawNeeded(SurfaceHolder holder) {
            mSurfaceView.requestRender();
            super.onSurfaceRedrawNeeded(holder);
        }

        @Override
        public void onVisibilityChanged(boolean visible) {
            super.onVisibilityChanged(visible);

            if (mSurfaceView != null) {
                if (visible)
                    mSurfaceView.onResume();
                else
                    mSurfaceView.onPause();
            }
        }

        private class JaggedSurfaceView extends GLSurfaceView {

            public JaggedSurfaceView(Context context) {
                super(context);
            }

            @Override
            public void surfaceCreated(SurfaceHolder holder) {

                super.surfaceCreated(holder);
            }

            @Override
            public SurfaceHolder getHolder() {
                return mEngine.getSurfaceHolder();
            }

            public void onDestroy() {
                super.onDetachedFromWindow();
            }
        }
    }
}
