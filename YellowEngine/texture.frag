#version 330 core

in vec3 v_Normal;
in vec3 v_FragPosition;
in vec2 v_TexCoord;

out vec4 o_FragColor;
 
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material u_Material;
uniform Light u_Light;
uniform vec3 u_CameraPosition;

void main()
{
	vec3 ambient = u_Light.ambient * texture(u_Material.diffuse, v_TexCoord).rgb;

	vec3 normal = normalize(v_Normal);
	vec3 lightDir = normalize(u_Light.position - v_FragPosition);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = u_Light.diffuse * diff * texture(u_Material.diffuse, v_TexCoord).rgb;

	vec3 cameraDir = normalize(u_CameraPosition - v_FragPosition);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(cameraDir, reflectDir), 0.0), u_Material.shininess);
	vec3 specular = u_Light.specular * spec * texture(u_Material.specular, v_TexCoord).rgb;

    o_FragColor = vec4(ambient + diffuse + specular, 1.0f);
}