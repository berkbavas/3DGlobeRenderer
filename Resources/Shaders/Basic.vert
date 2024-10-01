#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoords;

uniform mat3 normalMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

out vec4 fs_Position;
out vec2 fs_TextureCoords;
out vec3 fs_Normal;

void main()
{
    fs_Position = modelMatrix * position;
    fs_Normal = normalMatrix * normal;
    fs_TextureCoords = textureCoords;

    gl_Position = viewProjectionMatrix * fs_Position;
}
