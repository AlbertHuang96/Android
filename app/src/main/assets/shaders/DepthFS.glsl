#version 300 es

out vec4 FragColor;

uniform float zNear;
uniform float zFar;

float LinearizeDepth(float depth)
{
    // depth [0, 1]
    float z = depth * 2.0 - 1.0;
    return (2.0 * zNear * zFar) / (zFar + zNear - z * (zFar - zNear));
}

void main()
{
    float depth = LinearizeDepth(gl_FragCoord.z) / zFar;
    FragColor = vec4(vec3(depth), 1.0);
}