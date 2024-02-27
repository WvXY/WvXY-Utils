#version 460

layout(location = 0) in vec3 position;

layout(location = 0) out vec3 frag_color;

void main()
{
    gl_Position = vec4(position, 1.0);
    frag_color = vec3(1.0);
}
