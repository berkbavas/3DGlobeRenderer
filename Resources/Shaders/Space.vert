#version 450 core

layout (location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;

out vec3 fsTextureCoords;

void main()
{
    fsTextureCoords = position;
    gl_Position = projection * view * vec4(position, 1.0);
}  