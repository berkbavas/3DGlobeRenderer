#version 450 core

uniform samplerCube uTextureCube;
uniform float uBrightness;

in vec3 fsTextureCoords;
in vec4 fsCurrentClipPos;
in vec4 fsPrevClipPos;

layout(location = 0) out vec4 oColor;
layout(location = 1) out vec4 oGeodeticPosition;
layout(location = 2) out vec4 oVelocity;

void main()
{
    const vec3 Color = uBrightness * texture(uTextureCube, fsTextureCoords).rgb;
    oColor = vec4(Color, 1.0f);
    oGeodeticPosition = vec4(0);

    const vec2 CurrentNDC = fsCurrentClipPos.xy / fsCurrentClipPos.w;
    const vec2 PrevNDC    = fsPrevClipPos.xy    / fsPrevClipPos.w;
    oVelocity = vec4((CurrentNDC - PrevNDC) * 0.5f, 0.0f, 1.0f);
}
