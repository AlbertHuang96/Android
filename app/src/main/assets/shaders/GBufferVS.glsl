#version 300 es

precision mediump float;
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoord;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
            
uniform mat4 u_MVPMatrix;
uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
            
void main()
{
    TexCoords = a_texCoord;
    vec4 position = vec4(a_position, 1.0);
    gl_Position = u_ProjectionMatrix * u_ViewMatrix  * u_ModelMatrix * position;
    //gl_Position = u_MVPMatrix * position;
    FragPos = vec3(u_ModelMatrix * position);
    mat3 normalMatrix = transpose(inverse(mat3(u_ModelMatrix)));
    Normal = normalMatrix * a_normal;
}