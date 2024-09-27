#version 450 core

in vec3 fsTextureCoords;

out vec4 outColor;

uniform samplerCube skybox;
uniform float brightness = 0.5f;

void main()
{    
    vec4 color = texture(skybox, fsTextureCoords);
    outColor = vec4(brightness * color.x, brightness * color.y, brightness * color.z , 1.0f);
}