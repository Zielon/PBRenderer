#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 5) in float aId;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out V2G
{
	vec4 position;
	vec3 normal;
	flat int id;
	flat float mesh_id;
} vertex;

void main()
{
	vertex.position = projection * view * model * vec4(aPos, 1.0);
	vertex.normal = normalize(transpose(inverse(mat3(model))) * aNormal);
	vertex.mesh_id = aId;
	vertex.id = gl_VertexID; 
}