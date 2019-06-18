#version 330 core

in vec2 v_TexCoord;
in vec3 v_WorldFragPosition;
in vec3 v_Normal;

layout (location = 0) out vec3 o_WorldFragPosition;
layout (location = 1) out vec3 o_Diffuse;
layout (location = 2) out vec3 o_Normal;
layout (location = 3) out vec3 o_TexCoord;

struct Material
{
	sampler2D diffuse;
	float shininess;
};

uniform Material u_Material;

void main()
{
	o_WorldFragPosition = v_WorldFragPosition;
	o_Diffuse = vec3(texture(u_Material.diffuse, v_TexCoord));
	o_Normal = normalize(v_Normal);
	o_TexCoord = vec3(v_TexCoord, 0.0);
}