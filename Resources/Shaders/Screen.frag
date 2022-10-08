#version 430 core
in vec2 fsTextureCoords;

uniform sampler2D screenTexture;

out vec4 outColor;

void main()
{
    outColor = texture(screenTexture, fsTextureCoords);
}
