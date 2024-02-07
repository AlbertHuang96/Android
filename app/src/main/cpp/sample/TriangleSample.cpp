//
// Created by 86422 on 2024/2/1.
//

#include "TriangleSample.h"
#include "../util/UtilGL.h"
#include "../util/UtilLog.h"

TriangleSample::TriangleSample()
{
}

TriangleSample::~TriangleSample()
{
}

void TriangleSample::Init()
{
    if(rProgramID != 0)
        return;
    char vShaderStr[] =
            "#version 300 es                          \n"
            "layout(location = 0) in vec4 vPosition;  \n"
            "void main()                              \n"
            "{                                        \n"
            "   gl_Position = vPosition;              \n"
            "}                                        \n";

    char fShaderStr[] =
            "#version 300 es                              \n"
            "precision mediump float;                     \n"
            "out vec4 fragColor;                          \n"
            "void main()                                  \n"
            "{                                            \n"
            "   fragColor = vec4 ( 1.0, 0.0, 1.0, 1.0 );  \n"
            "}                                            \n";

    rProgramID = UtilGL::CreateProgram(vShaderStr, fShaderStr, rVertesShader, rFragmentShader);

}

void TriangleSample::Draw(int screenW, int screenH)
{
    LOGCATE("TriangleSample::Draw");
    GLfloat vVertices[] = {
            0.0f,  0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
    };

    if(rProgramID == 0)
        return;

    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);

    // Use the program object
    glUseProgram (rProgramID);

    // Load the vertex data
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
    glEnableVertexAttribArray (0);

    glDrawArrays (GL_TRIANGLES, 0, 3);
    glUseProgram (GL_NONE);

}

void TriangleSample::Destroy()
{
    if (rProgramID)
    {
        glDeleteProgram(rProgramID);
        rProgramID = GL_NONE;
    }
}

