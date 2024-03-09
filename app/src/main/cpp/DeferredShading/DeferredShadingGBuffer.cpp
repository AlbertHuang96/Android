//
// Created by 86422 on 2024/2/7.
//


#include <gtc/matrix_transform.hpp>
#include "DeferredShadingGBuffer.h"
#include "../util/UtilGL.h"

const GLenum attachments[3] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2
};

GBufferDeferredShading::GBufferDeferredShading()
{
    m_AngleX = 0;
    m_AngleY = 0;

    m_ScaleX = 1.0f;
    m_ScaleY = 1.0f;

    mBackpack = nullptr;
    mGbufferPass = nullptr;
    mLightingPass = nullptr;
    mLightBoxPass = nullptr;
}

GBufferDeferredShading::~GBufferDeferredShading()
{

}

void GBufferDeferredShading::Init(int screenW, int screenH)
{
    if(mBackpack != nullptr && mGbufferPass != nullptr)
        return;

    std::string path(DEFAULT_OGL_ASSETS_DIR);
    //mBackpack = new Model(path + "/model/poly/Apricot_02_hi_poly.obj");
    mBackpack = new Model(path + "/model/backpack/backpack.obj");

    //glEnable(GL_DEPTH_TEST);

    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenW, screenH, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
    // normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenW, screenH, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
    // color + specular color buffer
    glGenTextures(1, &gDiffuseSpec);
    glBindTexture(GL_TEXTURE_2D, gDiffuseSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenW, screenH, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gDiffuseSpec, 0);
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    //unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    /*glGenTextures(1, &gDepth);
    glBindTexture(GL_TEXTURE_2D, gDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT, NULL);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, gDepth, 0);*/

    glGenRenderbuffers(1, &gDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, gDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenW, screenH);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, gDepth);

    if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
        LOGCATE("GBufferDeferredShading::Init() GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    char vGbufferShaderStr[] =
            "#version 300 es\n"
            "precision mediump float;\n"
            "layout (location = 0) in vec3 a_position;\n"
            "layout (location = 1) in vec3 a_normal;\n"
            "layout (location = 2) in vec2 a_texCoord;\n"
            "out vec3 FragPos;\n"
            "out vec2 TexCoords;\n"
            "out vec3 Normal;\n"
            "\n"
            "uniform mat4 u_MVPMatrix;\n"
            "uniform mat4 u_ModelMatrix;\n"
            "uniform vec3 lightPos;\n"
            "uniform vec3 lightColor;\n"
            "uniform vec3 viewPos;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    TexCoords = a_texCoord;    \n"
            "    vec4 position = vec4(a_position, 1.0);\n"
            "    gl_Position = u_MVPMatrix * position;\n"
            "    FragPos = vec3(u_ModelMatrix * position);\n"
            "    mat3 normalMatrix = transpose(inverse(mat3(u_ModelMatrix)));\n"
            "    Normal = normalMatrix * a_normal;\n"
            "}";

    char fGbufferShaderStr[] =
            "#version 300 es\n"
            "precision mediump float;\n"
            "layout (location = 0) out vec3 gPosition;\n"
            "layout (location = 1) out vec3 gNormal;\n"
            "layout (location = 2) out vec4 gDiffuseSpec;\n"
            "\n"
            "in vec2 TexCoords;\n"
            "in vec3 FragPos;\n"
            "in vec3 Normal;\n"
            "\n"
            "uniform sampler2D texture_diffuse1;\n"
            "uniform sampler2D texture_specular1;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    // store the fragment position vector in the first gbuffer texture\n"
            "    gPosition = FragPos;\n"
            "    // also store the per-fragment normals into the gbuffer\n"
            "    gNormal = normalize(Normal);\n"
            "    // and the diffuse per-fragment color\n"
            "    gDiffuseSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;\n"
            "    // store specular intensity in gDiffuseSpec's alpha component\n"
            "    gDiffuseSpec.a = texture(texture_specular1, TexCoords).r;\n"
            "}";

    if (mBackpack->ContainsTextures())
    {
        mGbufferPass = new Shader(vGbufferShaderStr, fGbufferShaderStr);
    }
    else
    {
        //mGbufferPass = new Shader(vShaderStr, fNoTextureShaderStr);
    }

    char vLightingShaderStr[] =
            "#version 300 es\n"
            "precision mediump float;\n"
            "layout (location = 0) in vec3 a_position;\n"
            "layout (location = 1) in vec2 a_texCoord;\n"
            "out vec2 TexCoords;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    TexCoords = a_texCoord;    \n"
            "    gl_Position = vec4(a_position, 1.0);\n"
            "}";

    char fLightingShaderStr[] =
            "#version 300 es\n"
            "precision mediump float;\n"
            "in vec2 TexCoords;\n"
            "out vec4 FragColor;\n"
            "\n"
            "uniform vec3 lightPos;\n"
            "uniform vec3 lightColor;\n"
            "uniform vec3 viewPos;\n"
            "uniform float linear;\n"
            "uniform float quadratic;\n"
            "\n"
            "uniform sampler2D gPosition;\n"
            "uniform sampler2D gNormal;\n"
            "uniform sampler2D gDiffuseSpec;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    // retrieve data from gbuffer\n"
            "    vec3 FragPos = texture(gPosition, TexCoords).rgb;\n"
            "    vec3 Normal = texture(gNormal, TexCoords).rgb;\n"
            "    vec3 Diffuse = texture(gDiffuseSpec, TexCoords).rgb;\n"
            "    float Specular = texture(gDiffuseSpec, TexCoords).a;    \n"
            "\n"
            "    vec3 lighting  = Diffuse * 0.1; // hard-coded ambient component\n"
            "    vec3 viewDir  = normalize(viewPos - FragPos);\n"
            "    vec3 lightDir = normalize(lightPos - FragPos);\n"
            "    vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * lightColor;\n"
            "    // specular\n"
            "    vec3 halfwayDir = normalize(lightDir + viewDir);  \n"
            "    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);\n"
            "    vec3 specular = lightColor * spec * Specular;\n"
            "    // attenuation\n"
            "    //float distance = length(lightPos - FragPos);\n"
            "    //float attenuation = 1.0 / (1.0 + linear * distance + quadratic * distance * distance);\n"
            "    //diffuse *= attenuation;\n"
            "    //specular *= attenuation;\n"
            "    lighting += diffuse + specular;    \n"
            "    FragColor = vec4(lighting, 1.0);    \n"
            "\n"
            "}";

    mLightingPass = new Shader(vLightingShaderStr, fLightingShaderStr);
    //mLightingPass->use();
    //mLightingPass->setInt("gPosition", 0);
    //mLightingPass->setInt("gNormal", 1);
    //mLightingPass->setInt("gDiffuseSpec", 2);
}

void GBufferDeferredShading::RecreateFramebuffers(int screenW, int screenH)
{
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenW, screenH, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
    // normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenW, screenH, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
    // color + specular color buffer
    glGenTextures(1, &gDiffuseSpec);
    glBindTexture(GL_TEXTURE_2D, gDiffuseSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenW, screenH, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gDiffuseSpec, 0);
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    //unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    /*glGenTextures(1, &gDepth);
    glBindTexture(GL_TEXTURE_2D, gDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT, NULL);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, gDepth, 0);*/

    glGenRenderbuffers(1, &gDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, gDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenW, screenH);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, gDepth);

    if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
        LOGCATE("GBufferDeferredShading::Init() GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void GBufferDeferredShading::Draw(int screenW, int screenH)
{
    if(mBackpack == nullptr || mGbufferPass == nullptr) return;
    LOGCATE("GBufferDeferredShading::Draw()");

    //glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFrameBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glDrawBuffers(3, attachments);
    //glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);

    UpdateMVPMatrix(m_MVPMatrix, m_AngleX, m_AngleY, screenW, screenH);

    mGbufferPass->use();
    mGbufferPass->setMat4("u_MVPMatrix", m_MVPMatrix);
    mGbufferPass->setMat4("u_ModelMatrix", m_ModelMatrix);
    mGbufferPass->setVec3("lightPos", glm::vec3(0, 0, mBackpack->GetMaxViewDistance()));
    mGbufferPass->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    mGbufferPass->setVec3("viewPos", glm::vec3(0, 0, mBackpack->GetMaxViewDistance()));
    mBackpack->Draw((*mGbufferPass));
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT);
    mLightingPass->use();
    //glUseProgram(mLightingPass->ID);
    glActiveTexture(GL_TEXTURE0);
    mLightingPass->setInt("gPosition", 0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    //UtilGL::setInt(mLightingPass->ID, "gPosition", 0);

    glActiveTexture(GL_TEXTURE1);
    mLightingPass->setInt("gNormal", 1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    //UtilGL::setInt(mLightingPass->ID, "gNormal", 1);

    glActiveTexture(GL_TEXTURE2);
    mLightingPass->setInt("gDiffuseSpec", 2);
    glBindTexture(GL_TEXTURE_2D, gDiffuseSpec);
    //UtilGL::setInt(mLightingPass->ID, "gDiffuseSpec", 2);

    const float linear = 0.7f;
    const float quadratic = 1.8f;
    mLightingPass->setFloat("linear", linear);
    mLightingPass->setFloat("quadratic", quadratic);
    mLightingPass->setVec3("lightPos", glm::vec3(0, 0, mBackpack->GetMaxViewDistance()));
    mLightingPass->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    mLightingPass->setVec3("viewPos", glm::vec3(0, 0, mBackpack->GetMaxViewDistance()));

    RenderQuad();
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void *)0);

}

void GBufferDeferredShading::Destroy()
{
    LOGCATE("GBufferDeferredShading::Destroy");
    if (mBackpack != nullptr) {
        mBackpack->Destroy();
        delete mBackpack;
        mBackpack = nullptr;
    }

    if (mGbufferPass != nullptr) {
        mGbufferPass->Destroy();
        delete mGbufferPass;
        mGbufferPass = nullptr;
    }

    if (mLightingPass != nullptr) {
        mLightingPass->Destroy();
        delete mLightingPass;
        mLightingPass = nullptr;
    }

    if (mLightBoxPass != nullptr) {
        mLightBoxPass->Destroy();
        delete mLightBoxPass;
        mLightBoxPass = nullptr;
    }
}

void GBufferDeferredShading::UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, int screenW, int screenH)
{
    LOGCATE("GBufferDeferredShading::UpdateMVPMatrix angleX = %d, angleY = %d, screenW = %d, screenH = %d", angleX, angleY, screenW, screenH);
    angleX = angleX % 360;
    angleY = angleY % 360;

    //转化为弧度角
    float radiansX = static_cast<float>(MATH_PI / 180.0f * angleX);
    float radiansY = static_cast<float>(MATH_PI / 180.0f * angleY);


    // Projection matrix
    //glm::mat4 Projection = glm::ortho(-ratio, ratio, -1.0f, 1.0f, 0.1f, 100.0f);
    float ratio = 0.0;
    glm::mat4 Projection;
    if (screenW > screenH)
    {
        ratio = (float)screenW / screenH;
        Projection = glm::ortho(-ratio, ratio, -1.0f, 1.0f, -2.0f, 2.0f);
    }
    else
    {
        ratio = (float)screenH / screenW;
        Projection = glm::ortho(-1.0f, 1.0f, -ratio, ratio, -2.0f, 2.0f);
    }
    //lm::mat4 Projection = glm::frustum(-ratio, ratio, -1.0f, 1.0f, 1.0f, mBackpack->GetMaxViewDistance() * 4);
    //glm::mat4 Projection = glm::perspective(45.0f,ratio, 0.1f,100.f);

    // View matrix
    glm::mat4 View = glm::lookAt(
            glm::vec3(0, 0, mBackpack->GetMaxViewDistance() * 1.8f), // Camera is at (0,0,1), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Model matrix
    glm::mat4 Model = glm::mat4(1.0f);
    Model = glm::scale(Model, glm::vec3(0.6 * m_ScaleX, 0.6 * m_ScaleY, 1.0f));
    Model = glm::rotate(Model, radiansX, glm::vec3(1.0f, 0.0f, 0.0f));
    Model = glm::rotate(Model, radiansY, glm::vec3(0.0f, 1.0f, 0.0f));
    Model = glm::translate(Model, -mBackpack->GetAdjustModelPosVec());
    m_ModelMatrix = Model;
    //mvpMatrix = Projection * View * Model;
    mvpMatrix = Projection * Model;


}

void GBufferDeferredShading::UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY)
{
    RenderInterface::UpdateTransformMatrix(rotateX, rotateY, scaleX, scaleY);
    m_AngleX = static_cast<int>(rotateX);
    m_AngleY = static_cast<int>(rotateY);
    m_ScaleX = scaleX;
    m_ScaleY = scaleY;
}

void GBufferDeferredShading::RenderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}