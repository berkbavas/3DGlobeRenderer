#version 450 core

layout(location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 rotation;

out vec3 fs_TextureCoords;

void main()
{
    fs_TextureCoords = position;
    gl_Position = projection * rotation * vec4(position, 1.0);
}