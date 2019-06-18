#version 330 core

#define MAX_DIR_LIGHT 4
#define MAX_POINT_LIGHT 12
#define MAX_SPOT_LIGHT 4

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

in vec3 v_Normal;
in vec3 v_FragPosition;
in vec2 v_TexCoord;

out vec4 o_FragColor;

uniform Material u_Material;
uniform vec3 u_CameraPosition;

void main()
{
	o_FragColor = texture(u_Material.diffuse, v_TexCoord);
}