//
// Created by 86422 on 2024/2/7.
//

#ifndef RENDERPLAYGROUND_DEFERREDSHADINGGBUFFER_H
#define RENDERPLAYGROUND_DEFERREDSHADINGGBUFFER_H

#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>
#include <shader.cpp>
#include <model.cpp>
#include "RenderInterface.h"

class GBufferDeferredShading : public RenderInterface
{
public:
    GBufferDeferredShading();
    virtual ~GBufferDeferredShading();

    //virtual void LoadImage(NativeImage *pImage);

    virtual void Init();
    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

    void RenderQuad();

private:
    glm::mat4 m_MVPMatrix;
    glm::mat4 m_ModelMatrix;

    unsigned int gBuffer;
    unsigned int gDepth;
    unsigned int gPosition, gNormal, gDiffuseSpec;
    GLint defaultFrameBuffer = GL_NONE;

    unsigned int quadVAO = 0;
    unsigned int quadVBO = 0;

    Shader* mGbufferPass;
    Shader* mLightingPass;
    Shader* mLightBoxPass;

    Model* mBackpack;

    int m_AngleX;
    int m_AngleY;
    float m_ScaleX;
    float m_ScaleY;

    int SCR_WIDTH = 1080;
    int SCR_HEIGHT = 2029;
};

#endif //RENDERPLAYGROUND_DEFERREDSHADINGGBUFFER_H
