#version 420 core

out vec4 FragColor;

uniform vec3 camera_position;
uniform vec3 color;
uniform ivec3 triangle;

in G2P
{
	vec3 normal;
	flat ivec3 ids;
} frag;

void main()
{    
    vec3 ambient = vec3(0.05f, 0.05f, 0.05f);
    vec3 diffuse = vec3(0.4f, 0.4f, 0.4f);
   
	if (frag.ids.x == triangle.x && frag.ids.y == triangle.y && frag.ids.z == triangle.z)
	{
		FragColor = vec4(1.0f, 0.f, 0.f, 0.f);
		return;
	}

	vec3 normal = normalize(frag.normal);
	vec3 lightDir = normalize(camera_position);

	diffuse += diffuse * max(dot(normal, lightDir), 0.0) * color;
	
    FragColor = vec4(ambient + diffuse, 1.0);
}