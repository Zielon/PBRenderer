#version 330 core

out vec4 FragColor;

in vec3 frag_color;

void main()
{
    FragColor = vec4(frag_color, 1.0f);
}