#version 300 es

#ifdef GL_EXT_shader_pixel_local_storage
#extension GL_EXT_shader_pixel_local_storage : require
#endif

precision mediump float;

__pixel_local_outEXT FragDataLocal
{
    layout(r11f_g11f_b10f) mediump vec3 position;
    layout(rgb10_a2) mediump vec4 normal;
    layout(rgba8) mediump vec4 diffuseSpec;
    layout(r11f_g11f_b10f) mediump vec3 lighting;
} gBuf;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
            
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
            
void main()
{
    // store the fragment position vector in the first gbuffer texture
    gBuf.position = FragPos;

    // also store the per-fragment normals into the gbuffer
    gBuf.normal = vec4(normalize(Normal) * 0.5 + 0.5, 0.0);
    gBuf.lighting = vec3(0.0);

    gBuf.diffuseSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
    gBuf.diffuseSpec.a = texture(texture_specular1, TexCoords).r;
}