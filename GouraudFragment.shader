#version 330 core

out vec4 color;
in vec3 LightingColor;
uniform vec3 object_color;

void main()
{   
    color = vec4(LightingColor * object_color, 1.0f);   
}