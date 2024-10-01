#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoords;

uniform mat4 MVP;

out vec2 fs_TextureCoords;
out vec4 fs_Position;

void main()
{
    fs_Position = MVP * position;
    fs_TextureCoords = textureCoords;
    gl_Position = fs_Position;
}
