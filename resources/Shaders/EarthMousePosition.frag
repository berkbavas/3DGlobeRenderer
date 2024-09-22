#version 330 core

in vec2 fsTextureCoords;

layout(location = 0) out vec4 outLatLon;

void main()
{
    float latitude = 180.0f * (fsTextureCoords.t - 0.5f);
    float longitude = 360.0f * (fsTextureCoords.s - 0.5f);

    outLatLon = vec4(latitude, longitude, 0, 1);
}
