#version 450 core

in vec2 fsTextureCoords;
in vec4 fsPosition;

layout(location = 0) out vec3 outPosition;

void main()
{
    // float latitude = 180.0f * (fsTextureCoords.t - 0.5f);
    // float longitude = 360.0f * (fsTextureCoords.s - 0.5f);

    // outLatLon = vec4(latitude, longitude, 0, 1);

    outPosition = vec3(fsPosition);
}