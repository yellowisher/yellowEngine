#version 330

out vec4 o_FragColor;

struct SpotLight
{
	vec3 position;
	vec3 direction;

	float cutoffCos;
	float outerCutoffCos;

	vec3 color;
	float ambiendIntensity;
	float diffuseIntensity;
};

struct Attenuation
{
	float constant;
	float linear;
	float quadratic;
};

uniform sampler2D u_PositionMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_ColorMap;

uniform vec2 u_ScreenSize;
uniform SpotLight u_Light;
uniform Attenuation u_Attenuation;
uniform vec3 u_CameraPosition;

void main()
{
    vec2 texCoord      = gl_FragCoord.xy / u_ScreenSize;
	vec3 worldPosition = texture(u_PositionMap, texCoord).xyz;
	vec4 color         = texture(u_ColorMap, texCoord);
	vec3 normal        = normalize(texture(u_NormalMap, texCoord).xyz);

	vec3 fragToLightDir = u_Light.position.xyz - worldPosition;
	float dist = length(fragToLightDir);
	fragToLightDir = normalize(fragToLightDir);

	vec3 ambient = u_Light.color * u_Light.ambiendIntensity * color.rgb;

	float diff = max(dot(normal, fragToLightDir), 0.0);
	vec3 diffuse = u_Light.color * u_Light.diffuseIntensity * color.rgb * diff;

	vec3 fragToCamera = normalize(u_CameraPosition - worldPosition);
	vec3 halfVector = normalize(fragToLightDir + fragToCamera);
	// where to set shiness?
	float spec = clamp(pow(dot(normal, halfVector), 32), 0.0, 1.0);
	vec3 specular = vec3(color.a * spec);

	float theta = dot(-fragToLightDir, normalize(u_Light.direction)); 
	float epsilon = u_Light.cutoffCos - u_Light.outerCutoffCos;
	float intensity = clamp((theta - u_Light.outerCutoffCos) / epsilon, 0.0, 1.0);

	ambient *= intensity;
	diffuse *= intensity;

	vec3 combined = ambient + diffuse + specular;

	float attenuation = 
		u_Attenuation.constant + 
		dist * u_Attenuation.linear + 
		dist * dist * u_Attenuation.quadratic;

	o_FragColor = vec4(combined / attenuation, 1.0);
	//o_FragColor = vec4(vec3(1.0, 1.0, 1.0) + (combined / attenuation)*0.00001, 1.0);
}