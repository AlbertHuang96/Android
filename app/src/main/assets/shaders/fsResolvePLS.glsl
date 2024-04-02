#version 300 es

#ifdef GL_EXT_shader_pixel_local_storage
#extension GL_EXT_shader_pixel_local_storage : enable
#endif

precision mediump float;

__pixel_local_inEXT FragData
{
    layout(r11f_g11f_b10f) mediump vec3 position;
    layout(rgb10_a2) mediump vec4 normal;
    layout(rgba8) mediump vec4 diffuseSpec;
    layout(r11f_g11f_b10f) mediump vec3 lighting;
} gBuf;

out vec4 FragOutput;

void main()
{
    FragOutput = vec4(gBuf.lighting, 1.0);
}