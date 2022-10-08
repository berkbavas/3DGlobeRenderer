#version 330 core

struct Node {
    mat4 transformation;
    mat3 normalMatrix;
    vec4 color;
    float ambient;
    float diffuse;
    float specular;
    float shininess;
};

struct Sun {
    vec4 color;
    vec3 direction;
    float ambient;
    float diffuse;
    float specular;
};

uniform Node node;
uniform Sun sun;

uniform vec3 cameraPosition;

in vec3 fsPosition;
in vec2 fsTextureCoords;
smooth in vec3 fsNormal;

out vec4 outColor;


void main()
{
    vec3 sunDir = normalize(-sun.direction);
    vec3 viewDir = normalize(cameraPosition - fsPosition);

    // Ambient
    vec4 ambient = sun.ambient * node.ambient * node.color;

    // Diffuse
    float diffuseFactor = max(dot(fsNormal, sunDir), 0.0);
    vec4 diffuse = diffuseFactor * sun.diffuse * node.diffuse * node.color;

    // Specular
    vec3 reflectDir = reflect(-sunDir, fsNormal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), node.shininess);
    vec4 specular = specularFactor * sun.specular * node.specular * node.color;

    outColor = (ambient + diffuse + specular) * sun.color;
}
