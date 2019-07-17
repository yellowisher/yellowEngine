#version 330 core

in vec3 v_WorldFragPosition;
in vec2 v_TexCoord;
in vec3 v_Tangent;
in vec3 v_Bitangent;
in vec3 v_Normal;

layout (location = 0) out vec3 o_WorldFragPosition;
layout (location = 1) out vec3 o_Normal;
layout (location = 2) out vec4 o_DiffuseSpecular;

struct Material
{
	vec3 diffuseColor;
	vec3 specularColor;
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normal;
};

uniform Material u_Material;

void main()
{
	vec3 tangentNormal = texture(u_Material.normal, v_TexCoord).xyz;
	tangentNormal = normalize(tangentNormal * 2.0 - 1.0);

	mat3 TBN = mat3(normalize(v_Tangent), normalize(v_Bitangent), normalize(v_Normal));

	o_Normal			  = normalize(TBN * tangentNormal);
	o_WorldFragPosition   = v_WorldFragPosition;
	o_DiffuseSpecular.rgb = texture(u_Material.diffuse,  v_TexCoord).rgb * u_Material.diffuseColor;
	o_DiffuseSpecular.a   = texture(u_Material.specular, v_TexCoord).r * u_Material.specularColor.r;
}