#version 420 core

out vec4 FragColor;
in vec3 normal;

uniform vec3 direction;
uniform vec3 color;

void main()
{ 
    vec3 ambient = vec3(0.05f, 0.05f, 0.05f);
    vec3 diffuse = vec3(0.4f, 0.4f, 0.4f);
   
	vec3 n = normalize(normal);
	vec3 lightDir = normalize(-direction);

	diffuse += diffuse * max(dot(n, lightDir), 0.0) * color;
	
    FragColor = vec4(ambient + diffuse, 1.0);
}