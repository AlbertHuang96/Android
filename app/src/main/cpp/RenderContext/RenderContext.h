//
// Created by 86422 on 2024/1/17.
//

#ifndef RENDERPLAYGROUND_RENDERCONTEXT_H
#define RENDERPLAYGROUND_RENDERCONTEXT_H

#include <GLES3/gl3.h>
#include "RenderInterface.h"

class RenderContext
{
    RenderContext();
    ~RenderContext();
public:
    void OnSurfaceCreated();
    void OnSurfaceChanged(int width, int height);
    void OnDrawFrame();

    void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    static RenderContext* GetInstance();
    static void DestroyInstance();

    void SetImageData(int format, int width, int height, uint8_t *pData);

private:
    static RenderContext *mContext;
    RenderInterface* rCurrentSample;
    int rScreenW;
    int rScreenH;
};
#endif //RENDERPLAYGROUND_RENDERCONTEXT_H
