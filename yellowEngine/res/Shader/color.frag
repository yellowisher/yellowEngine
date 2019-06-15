#version 330 core

#define MAX_DIR_LIGHT 4
#define MAX_POINT_LIGHT 12
#define MAX_SPOT_LIGHT 4

struct Material
{
	float shininess;
};


/*
	vec3 in uniform buffer sometimes lead to wrong layout.
	So rather use vec4 you can find more information at https://www.khronos.org/opengl/wiki/Interface_Block_(GLSL)
*/
struct DirLight
{							// size		offset
	vec4 direction;			// 16		0

	vec4 ambient;			// 16		16
	vec4 diffuse;			// 16		32
	vec4 specular;			// 16		48
};							// 64		+64		

struct PointLight
{							// size		offset
	vec4 position;			// 16		0

	vec4 ambient;			// 16		16
	vec4 diffuse;			// 16		32
	vec4 specular;			// 16		48

	float constant;			// 4		64
	float linear;			// 4		68
	float quadratic;		// 4		72
};							// 76		+80

struct SpotLight
{							// size		offset
	vec4 position;			// 16		0
	vec4 direction;			// 16		16

	vec4 ambient;			// 16		32
	vec4 diffuse;			// 16		48
	vec4 specular;			// 16		64

	float constant;			// 4		80
	float linear;			// 4		84
	float quadratic;		// 4		88

	float cutoffCos;		// 4		92
	float outerCutoffCos;	// 4		96
};							// 100		+112

layout (std140) uniform LightBlock
{												// size		offset
	DirLight dirLights[MAX_DIR_LIGHT];			// 64*4		0
	PointLight pointLights[MAX_POINT_LIGHT];	// 80*12	256
	SpotLight spotLights[MAX_SPOT_LIGHT];		// 112*4	1216

	int dirLightCount;							// 4		1664
	int pointLightCount;						// 4		1668
	int spotLightCount;							// 4		1672

} Lights;										// 1680

//layout (std140) uniform CameraBlock
//{
//	vec3 position;
//
//} Camera;										// 16

float calcDiff(vec3 fragToLightDir);
float calcSpec(vec3 fragToLightDir);
vec3 calcDirLight(DirLight light);
vec3 calcPointLight(PointLight light);
vec3 calcSpotLight(SpotLight light);

in vec3 v_Normal;
in vec3 v_FragPosition;

out vec4 o_FragColor;

uniform Material u_Material;
uniform vec3 u_CameraPosition;

vec3 fragToCameraDir;

vec3 myColor = vec3(1.0, 1.0, 1.0);

void main()
{
	//fragToCameraDir = Camera.position - v_FragPosition;
	fragToCameraDir = normalize(u_CameraPosition - v_FragPosition);
	vec3 result = vec3(0.0, 0.0, 0.0);

	for(int i = 0; i < Lights.dirLightCount;	i++) result += calcDirLight(Lights.dirLights[i]);
	for(int i = 0; i < Lights.pointLightCount;	i++) result += calcPointLight(Lights.pointLights[i]);
	for(int i = 0; i < Lights.spotLightCount;	i++) result += calcSpotLight(Lights.spotLights[i]);

	o_FragColor = vec4(result, 1.0);
}


float calcDiff(vec3 fragToLightDir)
{
	return max(dot(v_Normal, fragToLightDir), 0.0);
}


float calcSpec(vec3 fragToLightDir)
{
	vec3 reflectDir = reflect(-fragToLightDir, v_Normal);
	return pow(max(dot(fragToCameraDir, reflectDir), 0.0), u_Material.shininess);
}


vec3 calcDirLight(DirLight light)
{
	vec3 fragToLightDir = -normalize(light.direction.xyz);

	float diff = calcDiff(fragToLightDir);
	float spec = calcSpec(fragToLightDir);

	vec3 ambient = light.ambient.rgb * myColor;
	vec3 diffuse = light.diffuse.rgb * diff * myColor;
	vec3 specular = light.specular.rgb * spec * myColor;

	return ambient + diffuse + specular;
}


vec3 calcPointLight(PointLight light)
{
	vec3 fragToLightDir = normalize(light.position.xyz - v_FragPosition);

	float diff = calcDiff(fragToLightDir);
	float spec = calcSpec(fragToLightDir);

	float dist = distance(light.position.xyz, v_FragPosition);
	float attenuation = 1.0 / (
		light.constant + 
		light.linear * dist +
		light.quadratic * dist * dist);

	vec3 ambient = light.ambient.rgb * myColor;
	vec3 diffuse = light.diffuse.rgb * diff * myColor;
	vec3 specular = light.specular.rgb * spec * myColor;

	return (ambient + diffuse + specular) * attenuation;
}


vec3 calcSpotLight(SpotLight light)
{
	vec3 fragToLightDir = normalize(light.position.xyz - v_FragPosition);

	float diff = calcDiff(fragToLightDir);
	float spec = calcSpec(fragToLightDir);

	float dist = distance(light.position.xyz, v_FragPosition);
	float attenuation = 1.0 / (
		light.constant + 
		light.linear * dist +
		light.quadratic * dist * dist);

	float theta = dot(-fragToLightDir, normalize(light.direction.xyz)); 
	float epsilon = light.cutoffCos - light.outerCutoffCos;
	float intensity = clamp((theta - light.outerCutoffCos) / epsilon, 0.0, 1.0);

	vec3 ambient = light.ambient.rgb * myColor;
	vec3 diffuse = light.diffuse.rgb * diff * myColor;
	vec3 specular = light.specular.rgb * spec * myColor;

	return (ambient + diffuse + specular) * attenuation * intensity;
}