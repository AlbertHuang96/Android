
#include "RenderContext.h"

#include "TriangleSample.h"
#include "Model3DSample.h"
#include "DeferredShadingGBuffer.h"

RenderContext* RenderContext::mContext = nullptr;

RenderContext::RenderContext()
{
    //rCurrentSample = new TriangleSample();
    rCurrentSample = new GBufferDeferredShading();
}


RenderContext::~RenderContext()
{
    if (rCurrentSample)
    {
        delete rCurrentSample;
        rCurrentSample = nullptr;
    }
}

RenderContext *RenderContext::GetInstance()
{
    //LOGCATE("MyGLRenderContext::GetInstance");
    if (mContext == nullptr)
    {
        mContext = new RenderContext();
    }
    return mContext;
}

void RenderContext::DestroyInstance()
{
    if (mContext)
    {
        delete mContext;
        mContext = nullptr;
    }
}

void RenderContext::UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY)
{
    LOGCATE("RenderContext::UpdateTransformMatrix [rotateX, rotateY, scaleX, scaleY] = [%f, %f, %f, %f]", rotateX, rotateY, scaleX, scaleY);
    if (rCurrentSample)
    {
        rCurrentSample->UpdateTransformMatrix(rotateX, rotateY, scaleX, scaleY);
    }
}

void RenderContext::OnSurfaceCreated()
{

    glClearColor(1.0f,1.0f,1.0f, 1.0f);
}

void RenderContext::OnSurfaceChanged(int width, int height)
{
    //LOGCATE("MyGLRenderContext::OnSurfaceChanged [w, h] = [%d, %d]", width, height);
    glViewport(0, 0, width, height);
    rScreenW = width;
    rScreenH = height;
}

void RenderContext::OnDrawFrame()
{
    //LOGCATE("MyGLRenderContext::OnDrawFrame");
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    if (rCurrentSample)
    {
        rCurrentSample->Init();
        rCurrentSample->Draw(rScreenW, rScreenH);
    }
}

void RenderContext::SetImageData(int format, int width, int height, uint8_t *pData)
{
    LOGCATE("RenderContext::SetImageData format=%d, width=%d, height=%d, pData=%p", format, width, height, pData);
    NativeImage nativeImage;
    nativeImage.format = format;
    nativeImage.width = width;
    nativeImage.height = height;
    nativeImage.ppPlane[0] = pData;

    switch (format)
    {
        case IMAGE_FORMAT_NV12:
        case IMAGE_FORMAT_NV21:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            break;
        case IMAGE_FORMAT_I420:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            nativeImage.ppPlane[2] = nativeImage.ppPlane[1] + width * height / 4;
            break;
        default:
            break;
    }

    if (rCurrentSample)
    {
        rCurrentSample->LoadImage(&nativeImage);
    }

}
