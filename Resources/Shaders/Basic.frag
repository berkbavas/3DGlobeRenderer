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

in vec4 fs_Position;
in vec2 fs_TextureCoords;
in vec3 fs_Normal;

out vec4 outColor;

void main()
{
    vec3 viewDirection = normalize(cameraPosition - fs_Position.xyz);

    float ambient = sun.ambient * globe.ambient;
    float diffuse = max(dot(fs_Normal, sun.direction), 0.0) * sun.diffuse * globe.diffuse;
    vec3 reflection = reflect(sun.direction, fs_Normal);

    vec3 halfwayDirection = normalize(sun.direction + viewDirection);
    float specular = pow(max(dot(fs_Normal, halfwayDirection), 0.0), globe.shininess) * globe.specular * sun.specular;

    outColor = (ambient + diffuse + specular) * sun.color * texture(globe.texture, fs_TextureCoords);
}
