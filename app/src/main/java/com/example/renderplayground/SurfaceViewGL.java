package com.example.renderplayground;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;

public class SurfaceViewGL extends GLSurfaceView {
    private RenderGL mRenderGL;

    private final float TOUCH_SCALE_FACTOR = 180.0f / 320;
    private float mPreviousY;
    private float mPreviousX;
    private int mXAngle;
    private int mYAngle;

    private float mPreScale = 1.0f;
    private float mCurScale = 1.0f;
    private long mLastMultiTouchTime;

    public SurfaceViewGL(Context context, RenderGL glRender) {
        this(context, glRender, null);
    }

    public SurfaceViewGL(Context context, RenderGL glRender, AttributeSet attrs) {
        super(context, attrs);
        this.setEGLContextClientVersion(3);
        mRenderGL = glRender;
        /*If no setEGLConfigChooser method is called,
        then by default the view will choose an RGB_888 surface with a depth buffer depth of at least 16 bits.*/
        setEGLConfigChooser(8, 8, 8, 8, 24, 8);
        setRenderer(mRenderGL);
        //setRenderMode(RENDERMODE_WHEN_DIRTY);
    }

    @Override
    public boolean onTouchEvent(MotionEvent e) {
        if (e.getPointerCount() == 1) {

            long currentTimeMillis = System.currentTimeMillis();
            if (currentTimeMillis - mLastMultiTouchTime > 200)
            {
                float y = e.getY();
                float x = e.getX();
                switch (e.getAction()) {
                    case MotionEvent.ACTION_MOVE:
                        float dy = y - mPreviousY;
                        float dx = x - mPreviousX;
                        mYAngle += dx * TOUCH_SCALE_FACTOR;
                        mXAngle += dy * TOUCH_SCALE_FACTOR;
                }
                mPreviousY = y;
                mPreviousX = x;

                mRenderGL.updateTransformMatrix(mXAngle, mYAngle, mCurScale, mCurScale);
            }

        }
        return true;
    }

}

