#version 450 core

uniform sampler2D uColorTexture;
uniform sampler2D uVelocityTexture;
uniform int  uMotionBlurEnabled;
uniform int  uMotionBlurSamples;
uniform float uMotionBlurStrength;

in vec2 fsTextureCoords;

out vec4 OutFragColor;

void main()
{
    vec3 Color = texture(uColorTexture, fsTextureCoords).rgb;

    if (uMotionBlurEnabled != 0)
    {
        const vec2 Velocity = texture(uVelocityTexture, fsTextureCoords).rg * uMotionBlurStrength;
        const int  Samples  = max(uMotionBlurSamples, 2);

        for (int i = 1; i < Samples; i++)
        {
            // Distribute samples symmetrically around the current position
            const float t      = float(i) / float(Samples - 1) - 0.5f;
            const vec2  Offset = Velocity * t;
            Color += texture(uColorTexture, fsTextureCoords + Offset).rgb;
        }
        Color /= float(Samples);
    }

    OutFragColor = vec4(clamp(Color, 0.0f, 1.0f), 1.0f);
}
