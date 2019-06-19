#version 330 core

in vec3 v_WorldFragPosition;
in vec3 v_Normal;
in vec2 v_TexCoord;

layout (location = 0) out vec3 o_WorldFragPosition;
layout (location = 1) out vec3 o_Normal;
layout (location = 2) out vec4 o_DiffuseSpecular;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

uniform Material u_Material;

void main()
{
	o_WorldFragPosition   = v_WorldFragPosition;
	o_Normal              = normalize(v_Normal);
	o_DiffuseSpecular.rgb = texture(u_Material.diffuse,  v_TexCoord).rgb;
	o_DiffuseSpecular.a   = texture(u_Material.specular, v_TexCoord).r;
}