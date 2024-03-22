//
// Created by 86422 on 2024/1/17.
//

#ifndef RENDERPLAYGROUND_RENDERCONTEXT_H
#define RENDERPLAYGROUND_RENDERCONTEXT_H

#include <GLES3/gl3.h>
#include <jni.h>
#include "RenderInterface.h"

class RenderContext
{
    RenderContext();
    ~RenderContext();
public:
    void OnSurfaceCreated(JNIEnv* env, jobject assetManager);
    void OnSurfaceChanged(int width, int height);
    void OnDrawFrame();

    void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    static RenderContext* GetInstance();
    static void DestroyInstance();

private:
    static RenderContext *mContext;
    RenderInterface* rCurrentSample;
    int rScreenW;
    int rScreenH;
};
#endif //RENDERPLAYGROUND_RENDERCONTEXT_H
