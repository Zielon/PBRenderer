#version 420 core

out vec4 FragColor;

uniform vec3 direction;
uniform vec3 color;
uniform ivec4 triangle;

in G2P
{
	vec3 normal;
	flat ivec4 ids;
} frag;

void main()
{       
	vec3 normal = normalize(frag.normal);
    FragColor = vec4(abs(normal), 1.0);
}