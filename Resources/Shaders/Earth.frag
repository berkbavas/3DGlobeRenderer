#version 330 core

struct Earth
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
    vec3 inverseDirection;
    float ambient;
    float diffuse;
    float specular;
};

uniform Earth earth;
uniform Sun sun;
uniform vec3 cameraPosition;

in vec4 fsPosition;
in vec2 fsTextureCoords;
in vec3 fsNormal;

out vec4 outColor;

void main()
{
    vec3 viewDirection = normalize(cameraPosition - fsPosition.xyz);

    float ambient = sun.ambient * earth.ambient;

    float diffuse = max(dot(fsNormal, sun.direction), 0.0) * sun.diffuse * earth.diffuse;

    vec3 reflection = reflect(sun.direction, fsNormal);
    float specular = pow(max(dot(viewDirection, reflection), 0.0), earth.shininess) * sun.specular * earth.specular;

    outColor = (ambient + diffuse + specular) * sun.color * texture(earth.texture, fsTextureCoords);
}
