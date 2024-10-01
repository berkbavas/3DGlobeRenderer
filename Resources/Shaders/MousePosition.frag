#version 450 core

in vec2 fs_TextureCoords;

out vec3 out_Position;

void main()
{
    float latitude = 180.0f * (fs_TextureCoords.t - 0.5f);
    float longitude = 360.0f * (fs_TextureCoords.s - 0.5f);
    out_Position = vec3(latitude, longitude, 0);
}
