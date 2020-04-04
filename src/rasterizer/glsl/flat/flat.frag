#version 420 core

out vec4 FragColor;

uniform vec3 camera_position;

in G2P
{
	vec3 normal;
} frag;

void main()
{    
    vec3 ambient = vec3(0.05f, 0.05f, 0.05f);
    vec3 diffuse = vec3(0.4f, 0.4f, 0.4f);
   
	vec3 normal = normalize(frag.normal);
	vec3 lightDir = normalize(camera_position);

	diffuse += diffuse * max(dot(normal, lightDir), 0.0) * vec3(0.75f, 0.75f, 0.75f);
	
    FragColor = vec4(ambient + diffuse, 1.0);
}