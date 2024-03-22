
#include "RenderContext.h"

#include "TriangleSample.h"
#include "Model3DSample.h"
#include "DeferredShadingGBuffer.h"

#include <UtilGL.h>

#include <vector>
#include <jni.h>

RenderContext* RenderContext::mContext = nullptr;

RenderContext::RenderContext()
{
    //rCurrentSample = new TriangleSample();
    rCurrentSample = new GBufferDeferredShading();

    //std::vector<int> tmpInt;
    //tmpInt.push_back(1);
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

void RenderContext::OnSurfaceCreated(JNIEnv* env, jobject assetManager)
{
    UtilGL::setEnvAndAssetManager(env, assetManager);
    glClearColor(1.0f,1.0f,1.0f, 1.0f);
}

void RenderContext::OnSurfaceChanged(int width, int height)
{
    //LOGCATE("MyGLRenderContext::OnSurfaceChanged [w, h] = [%d, %d]", width, height);
    glViewport(0, 0, width, height);
    rScreenW = width;
    rScreenH = height;
    rCurrentSample->RecreateFramebuffers(rScreenW, rScreenH);
}

void RenderContext::OnDrawFrame()
{
    //LOGCATE("MyGLRenderContext::OnDrawFrame");
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    if (rCurrentSample)
    {
        rCurrentSample->Init(rScreenW, rScreenH);
        rCurrentSample->Draw(rScreenW, rScreenH);
    }
}
