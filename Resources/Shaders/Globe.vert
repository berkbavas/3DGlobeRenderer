#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextureCoords;

uniform mat4 uModelMatrix;
uniform mat3 uNormalMatrix;
uniform mat4 uVP;

out vec3 fsWorldPosition;
out vec3 fsNormal;
out vec2 fsTextureCoords;

void main()
{
    fsWorldPosition = (uModelMatrix * vec4(aPosition, 1.0)).xyz;
    fsNormal = normalize(uNormalMatrix * aNormal);
    fsTextureCoords = aTextureCoords;

    gl_Position = uVP * vec4(fsWorldPosition, 1.0);
}
