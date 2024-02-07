package com.example.renderplayground;

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
}
