#version 300 es

#ifdef GL_EXT_shader_pixel_local_storage
#extension GL_EXT_shader_pixel_local_storage : enable
#endif

precision mediump float;

__pixel_localEXT FragData
{
    layout(r11f_g11f_b10f) mediump vec3 position;
    layout(rgb10_a2) mediump vec4 normal;
    layout(rgba8) mediump vec4 diffuseSpec;
    layout(r11f_g11f_b10f) mediump vec3 lighting;
} gBuf;

in vec2 TexCoords;
            
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform highp vec2 invViewport;
uniform highp mat4 invViewProj;
//uniform mat4 u_ViewMatrix;
//uniform mat4 u_ProjectionMatrix;

//uniform float linear;
//uniform float quadratic;
            
void main()
{

    vec3 FragPos = gBuf.position;
    vec3 Normal = gBuf.normal * 2.0 - 1.0;
    vec3 Diffuse = gBuf.diffuseSpec.rgb;
    float Specular = gBuf.diffuseSpec.a;

    //ivec2 coord = ivec2(gl_FragCoord.xy);
    //vec3 FragPos = texelFetch(gPosition, coord, 0).rgb;
    //vec3 Normal = texelFetch(gNormal, coord, 0).rgb;
    //vec3 Diffuse = texelFetch(gDiffuseSpec, coord, 0).rgb;
    //float Specular = texelFetch(gDiffuseSpec, coord, 0).a;

//gl_LastFragDepthARM float; reads the existing framebuffer depth value
    //highp vec4 clip;
    //clip.xy = gl_FragCoord.xy * invViewport;
    //clip.z = gl_LastFragDepthARM;
    //clip.w = 1.0;
    //clip = 2.0 * clip - 1.0;

    //highp mat4 invView = inverse(u_ViewMatrix);
    //highp mat4 invProj = inverse(u_ProjectionMatrix);
    //highp mat4 invViewProj = invView * invProj;
    //highp vec4 world = invViewProj * clip;
    //highp vec4 world = clip * invViewProj;
    //highp vec3 fragPos = world.xyz / world.w;
            
    vec3 lighting  = Diffuse * 0.1; // hard-coded ambient component
    //vec3 viewDir  = normalize(viewPos - fragPos);
    //vec3 lightDir = normalize(lightPos - fragPos);
    //vec3 viewDir  = normalize(viewPos - vec3(fragPos.xy, FragPos.z));
    //vec3 lightDir = normalize(lightPos - vec3(fragPos.xy, FragPos.z));

    vec3 viewDir  = normalize(viewPos - FragPos);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * lightColor;
    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir); 
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
    vec3 specular = lightColor * spec * Specular;
    // attenuation
    //float distance = length(lightPos - FragPos);
    //float attenuation = 1.0 / (1.0 + linear * distance + quadratic * distance * distance);
    //diffuse *= attenuation;
    //specular *= attenuation;
    
    lighting += diffuse + specular;

    gBuf.lighting = lighting;

            
}