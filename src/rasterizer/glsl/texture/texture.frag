#version 420 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{    
    FragColor = vec4(0.25, 0.25, 0.25, 1.0); // texture(texture_diffuse1, TexCoords);
}