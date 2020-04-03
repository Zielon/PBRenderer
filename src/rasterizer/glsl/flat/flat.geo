#version 420 core

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

in V2G
{
	vec4 position; 
	vec3 normal;

} vertices[3];

out G2P
{
	vec3 normal;
} vertex;

void main()
{	
	gl_Position = vertices[0].position;
	vertex.normal = vertices[0].normal;
	EmitVertex();

	gl_Position = vertices[1].position;
	vertex.normal = vertices[0].normal;
	EmitVertex();

	gl_Position = vertices[2].position;
	vertex.normal = vertices[0].normal;
	EmitVertex();

	EndPrimitive();
}