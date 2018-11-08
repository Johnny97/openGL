#version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 norms;

uniform mat4 model;
uniform mat4 view;
uniform mat4 pers;
uniform mat4 DepthBiasMVP;

out vec3 normal;
out vec3 fragment_position;
out vec4 ShadowCoord;

void main()
{
    gl_Position = pers *view * model * vec4(vertex_position,1);
    normal = vec3(transpose(inverse(model)) * vec4(norms, 1));
    fragment_position = vec3(model * vec4(vertex_position, 1));
	ShadowCoord = DepthBiasMVP * vec4(vertex_position, 1);
};