package com.example.renderplayground;

import android.app.Activity;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class RenderGL implements GLSurfaceView.Renderer {
    private RenderNative mNativeRender;
    //private AssetManager assetManager;
    private Activity mActivity;

    RenderGL(Activity activity) {
        mActivity = activity;
        //assetManager = activity.getAssets();
        mNativeRender = new RenderNative();
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        AssetManager assetManager = mActivity.getAssets();
        mNativeRender.native_OnSurfaceCreated(assetManager);
        //Log.e(TAG, "onSurfaceCreated() called with: GL_VERSION = [" + gl.glGetString(GL10.GL_VERSION) + "]");
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        mNativeRender.native_OnSurfaceChanged(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        mNativeRender.native_OnDrawFrame();
    }

    public void init() {
        mNativeRender.native_Init();
    }

    public void unInit() {
        mNativeRender.native_UnInit();
    }

    public void updateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY)
    {
        mNativeRender.native_UpdateTransformMatrix(rotateX, rotateY, scaleX, scaleY);
    }
}