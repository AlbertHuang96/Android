package com.example.renderplayground;

import android.graphics.Bitmap;

public class RenderNative {

    static {
        System.loadLibrary("RangerEngine");
    }

    public native void native_OnSurfaceCreated();

    public native void native_OnSurfaceChanged(int width, int height);

    public native void native_OnDrawFrame();

    public native void native_Init();

    public native void native_UnInit();

    public native void native_UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    public native void native_SetImageData(int format, int width, int height, byte[] bytes);

    public native int native_ImageProcess(Bitmap img);
}
