#version 450 core

layout(location = 0) in vec3 aPosition;

uniform mat4 uProjection;
uniform mat4 uRotation;
uniform mat4 uPrevRotation;

out vec3 fsTextureCoords;
out vec4 fsCurrentClipPos;
out vec4 fsPrevClipPos;

void main()
{
    fsTextureCoords  = aPosition;
    fsCurrentClipPos = uProjection * uRotation     * vec4(aPosition, 1.0);
    fsPrevClipPos    = uProjection * uPrevRotation * vec4(aPosition, 1.0);
    gl_Position = fsCurrentClipPos;
}