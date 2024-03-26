//
// Created by 86422 on 2024/2/7.
//

#ifndef RENDERPLAYGROUND_DEFERREDSHADINGGBUFFER_H
#define RENDERPLAYGROUND_DEFERREDSHADINGGBUFFER_H

#include <vector>

#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>
#include <shader.h>
#include <model.cpp>
#include "RenderInterface.h"

class GBufferDeferredShading : public RenderInterface
{
public:
    GBufferDeferredShading();
    virtual ~GBufferDeferredShading();

    //virtual void LoadImage(NativeImage *pImage);

    virtual void Init(int screenW, int screenH);
    virtual void Draw(int screenW, int screenH);

    virtual void RecreateFramebuffers(int screenW, int screenH);

    virtual void Destroy();

    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, int screenW, int screenH);

    void RenderQuad();
    void RenderQuadNoTexcoord();

private:
    glm::mat4 m_MVPMatrix;
    glm::mat4 m_ModelMatrix;

    float zNear = 0.0;
    float zFar = 0.0;

    unsigned int depthMapFBO;
    unsigned int depthMap;
    unsigned int dataBuffer;

    unsigned int gBuffer;
    unsigned int gDepth;
    unsigned int gPosition, gNormal, gDiffuseSpec;
    GLint defaultFrameBuffer = GL_NONE;

    unsigned int quadVAO = 0;
    unsigned int quadVBO = 0;

    Shader* mDepthPass;
    Shader* mDrawDepthPass;

    Shader* mGbufferPass;
    Shader* mLightingPass;
    Shader* mLightBoxPass;

    const char* GBufferVSShaderPath = "shaders/GBufferVS.glsl";
    const char* GBufferFSShaderPath = "shaders/GBufferFS.glsl";
    const char* LightingVSShaderPath = "shaders/LightingVS.glsl";
    const char* LightingFSShaderPath = "shaders/LightingFS.glsl";

    const char* QuadVSShaderPath = "shaders/QuadVS.glsl";
    const char* DepthFSShaderPath = "shaders/RenderDepthFS.glsl";

    const char* DepthVSShaderPath = "shaders/DepthVS.glsl";
    const char* SimpleDepthFSShaderPath = "shaders/DepthFS.glsl";

    Model* mBackpack;
    std::vector<glm::vec3> objectPositions;

    int m_AngleX;
    int m_AngleY;
    float m_ScaleX;
    float m_ScaleY;

    //int SCR_WIDTH = 1080;
    //int SCR_HEIGHT = 2029;
};

#endif //RENDERPLAYGROUND_DEFERREDSHADINGGBUFFER_H
