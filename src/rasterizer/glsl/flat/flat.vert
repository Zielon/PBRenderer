#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out V2G
{
	vec4 position;
	vec3 normal;
	int id;
} vertex;

void main()
{
	vertex.position = projection * view * model * vec4(aPos, 1.0);
	vertex.normal = normalize(mat3(model) * aNormal);
	vertex.id = gl_VertexID; 
}