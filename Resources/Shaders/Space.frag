#version 450 core

uniform samplerCube skybox;
uniform float brightness = 0.5f;

in vec3 fsTextureCoords;

out vec4 outColor;

void main()
{    
    vec4 color = texture(skybox, fsTextureCoords);
    outColor = vec4(brightness * color.x, brightness * color.y, brightness * color.z , 1.0f);
}