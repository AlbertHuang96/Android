#version 300 es

#ifdef GL_EXT_shader_framebuffer_fetch
#extension GL_EXT_shader_framebuffer_fetch : enable
//#extension GL_ARM_shader_framebuffer_fetch_depth_stencil : enable
#endif

precision mediump float;

in vec2 TexCoords;
out vec4 FragColor;
            
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform float linear;
uniform float quadratic;

#ifdef GL_EXT_shader_framebuffer_fetch
layout (location = 0) inout vec3 gPosition;
layout (location = 1) inout vec3 gNormal;
layout (location = 2) inout vec4 gDiffuseSpec;
#else
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gDiffuseSpec;
#endif
            
void main()
{
    #ifdef GL_EXT_shader_framebuffer_fetch
    vec3 FragPos = gPosition;
    vec3 Normal = gNormal;
    vec3 Diffuse = gDiffuseSpec.rgb;
    float Specular = gDiffuseSpec.a;

    // gles 3.0
    // The EXT_shader_framebuffer_fetch extension works differently.
    // GLSL ES 3.0 removes the gl_FragData and gl_FragColor builtin fragment output variables in favor of requiring fragment outputs to be declared in the shader.
    // Correspondingly, the gl_LastFragData builtin variable is not present in GLSL ES 3.0 fragment shaders.
    // Instead, any fragment output variables you declare with the inout qualifier contain previous fragment data when the shader runs. For more details,
    // see Fetch Framebuffer Data for Programmable Blending.

    //vec3 FragPos = gl_LastFragData[0].rgb;
    //vec3 Normal = gl_LastFragData[1].rgb;
    //vec3 Diffuse = gl_LastFragData[2].rgb;
    //float Specular = gl_LastFragData[2].a;
    #else
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gDiffuseSpec, TexCoords).rgb;
    float Specular = texture(gDiffuseSpec, TexCoords).a;
    #endif
            
    vec3 lighting  = Diffuse * 0.1; // hard-coded ambient component
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
    FragColor = vec4(lighting, 1.0);   
            
}