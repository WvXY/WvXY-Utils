#version 460

layout (location = 0) in vec3 fColor;

out vec4 oColor;

void main()
{
    oColor = vec4(fColor, 1.0f);
}
