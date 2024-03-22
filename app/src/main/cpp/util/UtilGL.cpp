//
// Created by 86422 on 2024/2/1.
//

#include "UtilGL.h"
#include "UtilLog.h"
#include <stdlib.h>
#include <cstring>

#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>

static JNIEnv* env = nullptr;
static jobject assetManager = nullptr;

void UtilGL::setEnvAndAssetManager(JNIEnv* envParam, jobject assetManagerParam)
{
    env = envParam;
    assetManager = assetManagerParam;
}

static AAsset* loadAsset(const char* filePath)
{
    AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);

    if (mgr == nullptr)
    {
        return nullptr;
    }
    // assets' operation are not thread-safe
    // mutex isnt copyable
    // error: call to implicitly-deleted copy constructor
    //std::mutex mLock;
    return AAssetManager_open ( mgr, filePath, AASSET_MODE_STREAMING );
}

char* UtilGL::openTextFile(const char* path)
{
    char* buffer;
    AAsset* asset = loadAsset(path);
    if (!asset)
    {
        LOGCATE ( "UtilGL::openTextFile Can't open file %s.", path );
        return nullptr;
    }

    size_t size = static_cast<size_t> ( AAsset_getLength ( asset ) );
    LOGCATE ( "UtilGL::openTextFile size %ld.", size );
    buffer = new char[size + 1];
    int num = AAsset_read( asset, buffer, size );
    AAsset_close ( asset );

    LOGCATE ( "UtilGL::openTextFile num %d.", num );

    if ( size != num )
    {
        LOGCATE ( "UtilGL::openTextFile Can't read file %s.", path );
        delete[] buffer;
        return nullptr;
    }

    buffer[size] = '\0';
    return buffer;
}

GLuint UtilGL::LoadShader(GLenum shaderType, const char *pSource)
{
    GLuint shader = 0;
    //FUN_BEGIN_TIME("GLUtils::LoadShader")
    shader = glCreateShader(shaderType);
    if (shader)
    {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen)
            {
                char* buf = (char*) malloc((size_t)infoLen);
                if (buf)
                {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGCATE("UtilGL::LoadShader Could not compile shader %d:\n%s\n", shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    //FUN_END_TIME("GLUtils::LoadShader")
    return shader;
}

GLuint UtilGL::CreateProgram(const char *pVertexShaderSource, const char *pFragShaderSource, GLuint &vertexShaderHandle, GLuint &fragShaderHandle)
{
    GLuint program = 0;
    //FUN_BEGIN_TIME("GLUtils::CreateProgram")
    vertexShaderHandle = LoadShader(GL_VERTEX_SHADER, pVertexShaderSource);
    if (!vertexShaderHandle)
    {
        return program;
    }

    fragShaderHandle = LoadShader(GL_FRAGMENT_SHADER, pFragShaderSource);
    if (!fragShaderHandle)
    {
        return program;
    }

    program = glCreateProgram();
    if (program)
    {
        glAttachShader(program, vertexShaderHandle);
        CheckGLError("glAttachShader");
        glAttachShader(program, fragShaderHandle);
        CheckGLError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

        glDetachShader(program, vertexShaderHandle);
        glDeleteShader(vertexShaderHandle);
        vertexShaderHandle = 0;
        glDetachShader(program, fragShaderHandle);
        glDeleteShader(fragShaderHandle);
        fragShaderHandle = 0;
        if (linkStatus != GL_TRUE)
        {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength)
            {
                char* buf = (char*) malloc((size_t)bufLength);
                if (buf)
                {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGCATE("UtilGL::CreateProgram Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    //FUN_END_TIME("GLUtils::CreateProgram")
    LOGCATE("UtilGL::CreateProgram program = %d", program);
    return program;
}

GLuint UtilGL::CreateProgram(const char *pVertexShaderSource, const char *pFragShaderSource) {
    GLuint vertexShaderHandle, fragShaderHandle;
    return CreateProgram(pVertexShaderSource, pFragShaderSource, vertexShaderHandle, fragShaderHandle);
}

void UtilGL::DeleteProgram(GLuint &program)
{
    LOGCATE("UtilGL::DeleteProgram");
    if (program)
    {
        glUseProgram(0);
        glDeleteProgram(program);
        program = 0;
    }
}

void UtilGL::CheckGLError(const char *pGLOperation)
{
    for (GLint error = glGetError(); error; error = glGetError())
    {
        LOGCATE("UtilGL::CheckGLError GL Operation %s() glError (0x%x)\n", pGLOperation, error);
    }

}