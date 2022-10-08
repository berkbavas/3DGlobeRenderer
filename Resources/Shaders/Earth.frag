#version 330 core

struct Earth {
    mat4 transformation;
    mat3 normalMatrix;
    float ambient;
    float diffuse;
    float specular;
    float shininess;
    sampler2D color;
    sampler2D height;
};

struct Sun {
    vec4 color;
    vec3 direction;
    float ambient;
    float diffuse;
    float specular;
};

uniform Earth earth;
uniform vec3 cameraPosition;
uniform Sun sun;

in vec3 fsPosition;
in vec2 fsTextureCoords;
smooth in vec3 fsNormal;

out vec4 outColor;

void main()
{
    vec3 sunDir = normalize(-sun.direction);
    vec3 viewDir = normalize(cameraPosition - fsPosition);

    // Ambient
    float ambient = sun.ambient * earth.ambient;

    // Diffuse
    float diffuse = max(dot(fsNormal, sunDir), 0.0) * sun.diffuse * earth.diffuse;

    // Specular
    vec3 reflectDir = reflect(-sunDir, fsNormal);
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), earth.shininess) * sun.specular * earth.specular;

    outColor = (ambient + diffuse + specular) * sun.color * texture(earth.color, fsTextureCoords);
}
