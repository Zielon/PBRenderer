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
    vec3 ambient = vec3(0.05f, 0.05f, 0.05f);
    vec3 diffuse = vec3(0.4f, 0.4f, 0.4f);
   
	if (all(equal(frag.ids, triangle)))
	{
		FragColor = vec4(1.0f, 0.f, 0.f, 0.f);
		return;
	}

	vec3 normal = normalize(frag.normal);
	vec3 lightDir = normalize(-direction);

	diffuse += diffuse * max(dot(normal, lightDir), 0.0) * color;
	
    FragColor = vec4(ambient + diffuse, 1.0);
}