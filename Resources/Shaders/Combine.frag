#version 450 core

in vec2 fs_TextureCoords;

uniform sampler2D colorTexture;
uniform sampler2D velocityTexture;
uniform int numberOfSamples;

layout(location = 0) out vec4 out_Color;

void main()
{
    float weight = numberOfSamples;
    vec4 color = weight * texture(colorTexture, fs_TextureCoords);
    vec4 velocity = texture(velocityTexture, fs_TextureCoords);
    vec2 texCoord = fs_TextureCoords;
    texCoord += velocity.xy;
    float totalWeight = weight;

    for (int i = 1; i < numberOfSamples; ++i, texCoord += velocity.xy)
    {
        weight--;
        vec4 currentColor = weight * texture(colorTexture, texCoord);
        color += currentColor;
        totalWeight += weight;
    }

    out_Color = color / totalWeight;
}
