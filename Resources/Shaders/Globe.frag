#version 450 core

struct Globe
{
    float Ambient;
    float Diffuse;
    float Specular;
    float Shininess;
    sampler2D Texture;
};

struct Sun
{
    vec3 Color;
    vec3 Direction;
    float Ambient;
    float Diffuse;
    float Specular;
};

uniform Globe uGlobe;
uniform Sun uSun;
uniform vec3 uCameraPosition;

in vec3 fsWorldPosition;
in vec3 fsNormal;
in vec2 fsTextureCoords;

vec3 ProcessLighting()
{
    vec3 ViewDirection = normalize(uCameraPosition - fsWorldPosition);

    const float Ambient = uSun.Ambient * uGlobe.Ambient;
    const float Diffuse = max(dot(fsNormal, uSun.Direction), 0.0f) * uSun.Diffuse * uGlobe.Diffuse;
    const vec3 Reflection = reflect(uSun.Direction, fsNormal);

    const vec3 HalfwayDirection = normalize(uSun.Direction + ViewDirection);
    const float Specular = pow(max(dot(fsNormal, HalfwayDirection), 0.0f), uGlobe.Shininess) * uGlobe.Specular * uSun.Specular;
    return (Ambient + Diffuse + Specular) * uSun.Color;
}

layout(location = 0) out vec4 oColor;
layout(location = 1) out vec4 oGeodeticPosition;

void main()
{
    const vec3 Lighting = ProcessLighting();
    const vec4 TextureColor = texture(uGlobe.Texture, fsTextureCoords);
    oColor = vec4(Lighting, 1.0f) * TextureColor;

    const float Latitude = 180.0f * (0.5f - fsTextureCoords.t);
    const float Longitude = 360.0f * (fsTextureCoords.s - 0.5f);

    oGeodeticPosition = vec4(Latitude, Longitude, 0.0f, 1.0f);
}
