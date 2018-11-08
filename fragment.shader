#version 330 core

out vec4 color;

#define NR_POINT_LIGHTS 4
#define NR_COLOR_LIGHTS 4


in vec3 normal;
in vec3 fragment_position;
in vec4 ShadowCoord;

uniform vec3 object_color;
uniform vec3 light_effect;
uniform sampler2DShadow shadowMap;

const vec3 LightPosition[NR_POINT_LIGHTS] = vec3[NR_POINT_LIGHTS](
	vec3(10.0f, 15.0f, 5.0f),
	vec3(-10.0f, 15.0f, 5.0f),
	vec3(0.0f, 15.0f, 5.0f),
	vec3(0.0f, 0.0f, 25.0f)
	);

const vec3 LightColor[NR_COLOR_LIGHTS] = vec3[NR_COLOR_LIGHTS](
	vec3(0.2f, 0.05f, 0.05f),
	vec3(0.05f, 0.2f, 0.05f),
	vec3(0.05f, 0.05f, 0.2f),
	vec3(0.05f, 0.05f, 0.05f)
	);

void main()
{

	vec3 result;
	vec3 viewPos = vec3(0.0f, 0.0f, 3.0f);
	float visibility = texture(shadowMap, vec3(ShadowCoord.xy, (ShadowCoord.z) / ShadowCoord.w));

	for (int i = 0; i < NR_POINT_LIGHTS; i++) 
	{
		//ambient
		   float amb = 0.25f;
		   vec3 ambient = amb * LightColor[i];

		//diffuse
		vec3 light_direction = normalize((LightPosition[i]) - fragment_position);
		float diff = max(dot(normal, light_direction), 0.0);
		vec3 diffuse = diff * LightColor[i];

		//specular
		   float specularStrength = 1.0f;
		   vec3 viewDir = normalize(viewPos - fragment_position);
		   vec3 reflectDir = reflect(-light_direction, normal);  
		   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
		   vec3 specular = specularStrength * spec * LightColor[i];

		   
		   result +=(ambient + diffuse + specular);
	}

	//result = visibility * result;

	color = vec4(result * object_color, 1.0f);
	//if(light_effect == vec3(1,1,0))
	//{
	//	//light off
	//	color = vec4(object_color, 1.0f);
	//}
	//if (light_effect == vec3(1, 1, 1))
	//{
	//	//normal on
	//	color = vec4(normal, 1.0f);
	//}
	//if (light_effect == vec3(1, 0, 0))
	//{
	//	//light on
	//	color = vec4((ambient + diffuse + specular) * object_color, 1.0f);
	//}
	//
}