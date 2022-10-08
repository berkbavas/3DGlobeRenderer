#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoords;

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

uniform Earth earth;
uniform mat4 VP;

out vec3 fsPosition;
out vec2 fsTextureCoords;
smooth out vec3 fsNormal;

void main()
{
//    float height = texture(earth.height, textureCoords).r;
//    vec3  newPosition = position + 0.25 * height * normal;

    fsPosition = vec3(earth.transformation * vec4(position, 1.0));
    fsNormal = earth.normalMatrix * normal;
    fsTextureCoords = textureCoords;
    gl_Position = VP * vec4(fsPosition, 1.0);
}
