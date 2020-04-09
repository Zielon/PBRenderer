#version 420 core

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

in V2G
{
	vec4 position; 
	vec3 normal;
	int id;
} vertices[3];

out G2P
{
	vec3 normal;
	flat ivec3 ids; 
} vertex;

void main()
{	
	vertex.ids = ivec3(vertices[0].id, vertices[1].id, vertices[2].id); 

	// Average normal for face to achieve flat shading 
	vec3 n = normalize((vertices[0].normal + vertices[1].normal + vertices[2].normal) / 3.0);

	gl_Position = vertices[0].position;
	vertex.normal = n;
	EmitVertex();

	gl_Position = vertices[1].position;
	vertex.normal = n;
	EmitVertex();

	gl_Position = vertices[2].position;
	vertex.normal = n;
	EmitVertex();
}