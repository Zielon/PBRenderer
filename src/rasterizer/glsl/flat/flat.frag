#version 420 core

out vec4 FragColor;

in G2P
{
	vec3 normal;
} frag;

void main()
{    
	vec3 lights[4];

	lights[0] = vec3(1.f, -1.f, 0.f);
	lights[1] = vec3(1.f, 1.f, 0.f);
	lights[2] = vec3(-1.f, 1.f, 0.f);
	lights[3] = vec3(-1.f, -1.f, 0.f);

	vec3 diffuse = vec3(0.f, 0.f, 0.f);

	for(int i = 0; i < 4; i++)
	{
		vec3 light = lights[i];
		vec3 normal = normalize(frag.normal);
		vec3 lightDir = normalize(-light);
		diffuse += vec3(1.f, 1.f, 1.f) * max(dot(normal, lightDir), 0.0) * vec3(0.75f, 0.75f, 0.75f);
	}

    FragColor = vec4(diffuse, 1.0);
}