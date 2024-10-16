#version 450 core

uniform samplerCube skybox;
uniform float brightness = 0.5f;

in vec3 fs_TextureCoords;

layout(location = 0) out vec4 out_Color;
layout(location = 1) out vec4 out_Velocity;

void main()
{
    vec4 color = texture(skybox, fs_TextureCoords);
    out_Color = vec4(brightness * color.x, brightness * color.y, brightness * color.z, 1.0f);
    out_Velocity = vec4(0);
}