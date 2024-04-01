#version 300 es

#ifdef GL_EXT_shader_framebuffer_fetch
#extension GL_EXT_shader_framebuffer_fetch : enable
#endif

precision mediump float;

#ifdef GL_EXT_shader_framebuffer_fetch
layout (location = 0) inout vec3 gPosition;
layout (location = 1) inout vec3 gNormal;
layout (location = 2) inout vec4 gDiffuseSpec;
//layout (location = 3) inout vec4 gOutput;
#else
//layout (location = 0) out vec3 gPosition;
//layout (location = 1) out vec3 gNormal;
//layout (location = 2) out vec4 gDiffuseSpec;
#endif

            
in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
            
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
            
void main()
{
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);
    // and the diffuse per-fragment color
    gDiffuseSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
    // store specular intensity in gDiffuseSpec's alpha component
    gDiffuseSpec.a = texture(texture_specular1, TexCoords).r;
    //gOutput = vec4(1.0);
}