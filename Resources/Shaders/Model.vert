#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoords;

struct Node {
    mat4 transformation;
    mat3 normalMatrix;
    vec4 color;
    float ambient;
    float diffuse;
    float specular;
    float shininess;
};

uniform Node node;
uniform mat4 VP;

out vec3 fsPosition;
out vec2 fsTextureCoords;
smooth out vec3 fsNormal;

void main()
{
    fsPosition = vec3(node.transformation * vec4(position, 1.0));
    fsNormal = node.normalMatrix * normal;
    fsTextureCoords = textureCoords;
    gl_Position = VP * vec4(fsPosition, 1.0);
}
