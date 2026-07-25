#version 450 core

layout(location = 0) in vec3 aPosition;

uniform mat4 uProjection;
uniform mat4 uRotation;

out vec3 fsTextureCoords;

void main()
{
    fsTextureCoords = aPosition;
    gl_Position = uProjection * uRotation * vec4(aPosition, 1.0);
}