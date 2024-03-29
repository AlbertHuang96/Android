#version 300 es
precision mediump float;
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texCoord;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
            
void main()
{
    
    gl_Position = u_ProjectionMatrix * u_ViewMatrix  * u_ModelMatrix * vec4(a_position, 1.0);
}