#version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 norms;

uniform mat4 model;
uniform mat4 view;
uniform mat4 pers;

out vec3 LightingColor;

void main()
{
    gl_Position = pers *view * model * vec4(vertex_position,1);
	//gouraud
    vec3 normal = vec3(transpose(inverse(model)) * vec4(norms, 1));
    vec3 fragment_position = vec3(model * vec4(vertex_position, 1));

	vec3 viewPos = vec3(0.0f, 0.0f, 3.0f);
	vec3 light_position = vec3(0.0f, 20.0f, 5.0f);
	vec3 light_color = vec3(0.8f, 0.8f, 0.8f);

	//ambient
	float amb = 0.25f;
	vec3 ambient = amb * light_color;

	//diffuse
	vec3 light_direction = normalize(light_position - fragment_position);
	float diff = max(dot(normal, light_direction), 0.0);
	vec3 diffuse = diff * light_color;

	//specular
	float specularStrength = 1.0f;
	vec3 viewDir = normalize(viewPos - fragment_position);
	vec3 reflectDir = reflect(-light_direction, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = specularStrength * spec * light_color;

	LightingColor = ambient + diffuse + specular;
};