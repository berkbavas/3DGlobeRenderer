#version 450 core

uniform samplerCube uTextureCube;
uniform float uBrightness;

in vec3 fsTextureCoords;

layout(location = 0) out vec4 oColor;
layout(location = 1) out vec4 oGeodeticPosition;

void main()
{
    const vec3 Color = uBrightness * texture(uTextureCube, fsTextureCoords).rgb;
    oColor = vec4(Color, 1.0f);
    oGeodeticPosition = vec4(0);
}
