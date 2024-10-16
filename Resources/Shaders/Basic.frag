#version 450 core

struct Globe
{
    float ambient;
    float diffuse;
    float specular;
    float shininess;
    sampler2D texture;
};

struct Sun
{
    vec4 color;
    vec3 direction;
    float ambient;
    float diffuse;
    float specular;
};

uniform Globe globe;
uniform Sun sun;
uniform vec3 cameraPosition;
uniform mat4 previousViewProjectionMatrix;
uniform mat4 viewProjectionMatrix;

in vec4 fs_Position;
in vec2 fs_TextureCoords;
in vec3 fs_Normal;

layout(location = 0) out vec4 out_Color;
layout(location = 1) out vec4 out_Velocity;

void main()
{
    vec3 viewDirection = normalize(cameraPosition - fs_Position.xyz);

    float ambient = sun.ambient * globe.ambient;
    float diffuse = max(dot(fs_Normal, sun.direction), 0.0) * sun.diffuse * globe.diffuse;
    vec3 reflection = reflect(sun.direction, fs_Normal);

    vec3 halfwayDirection = normalize(sun.direction + viewDirection);
    float specular = pow(max(dot(fs_Normal, halfwayDirection), 0.0), globe.shininess) * globe.specular * sun.specular;

    out_Color = (ambient + diffuse + specular) * sun.color * texture(globe.texture, fs_TextureCoords);

    vec4 prevProj = previousViewProjectionMatrix * fs_Position;
    prevProj /= prevProj.w;

    vec4 currProj = viewProjectionMatrix * fs_Position;
    currProj /= currProj.w;

    float dx = currProj.x - prevProj.x;
    float dy = currProj.y - prevProj.y;

    out_Velocity = vec4(dx, dy, 0, 0);
}
