#version 330

out vec4 o_FragColor;

struct SpotLight
{
	vec3 position;
	vec3 direction;

	float cutoffCos;
	float outerCutoffCos;

	vec3 color;
	float ambientIntensity;
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
uniform mat4 u_LightProjView;
uniform sampler2D u_ShadowMap;

uniform SpotLight u_Light;
uniform vec3 u_CameraPosition;
uniform Attenuation u_Attenuation;
uniform vec2 u_ScreenSize;

const float EPSILON_FACTOR = 0;
const float EPSILON_MAX = 0.00001;

const int SHADOW_SAMPLE_RANGE = 1;
const int NUM_SHADOW_SAMPLE = (2 * SHADOW_SAMPLE_RANGE + 1) * (2 * SHADOW_SAMPLE_RANGE + 1);

void main()
{
	vec2 texCoord	   = gl_FragCoord.xy / u_ScreenSize;
	vec3 worldPosition = texture(u_PositionMap, texCoord).xyz;
	vec4 color         = texture(u_ColorMap, texCoord);
	vec3 normal        = normalize(texture(u_NormalMap, texCoord).xyz);


	// Shading
	vec3 fragToLightDir = u_Light.position - worldPosition;
	float dist = length(fragToLightDir);
	fragToLightDir = normalize(fragToLightDir);
	float normDotFragToLight = max(dot(normal, fragToLightDir), 0.0);

	vec3 ambient = u_Light.color * u_Light.ambientIntensity * color.rgb;

	float diff = normDotFragToLight;
	vec3 diffuse = u_Light.color * u_Light.diffuseIntensity * color.rgb * diff;

	vec3 fragToCamera = normalize(u_CameraPosition - worldPosition);
	vec3 halfVector = normalize(fragToLightDir + fragToCamera);

	float spec = clamp(pow(dot(normal, halfVector), 32), 0.0, 1.0);
	vec3 specular = vec3(color.a * spec * u_Light.diffuseIntensity);

	// should make sure where is Z+ direction...
	float theta = dot(-fragToLightDir, u_Light.direction); 
	float epsilon = u_Light.cutoffCos - u_Light.outerCutoffCos;
	float intensity = clamp((theta - u_Light.outerCutoffCos) / epsilon, 0.0, 1.0);

	diffuse *= intensity;
	specular *= intensity;

	// Shadow
	vec4 lightPosition = u_LightProjView * vec4(worldPosition, 1.0);
	vec3 shadowCoord   = lightPosition.xyz / lightPosition.w;
	shadowCoord = shadowCoord * 0.5 + 0.5;

	float shadow = 0;
	if(shadowCoord.z < 1.0)
	{
		float currentDepth = shadowCoord.z;

		vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
		for(int dx = -SHADOW_SAMPLE_RANGE; dx <= SHADOW_SAMPLE_RANGE; dx++)
		{
			for(int dy = -SHADOW_SAMPLE_RANGE; dy <= SHADOW_SAMPLE_RANGE; dy++)
			{
				float visibleDepth = texture(u_ShadowMap, shadowCoord.xy + vec2(dx, dy) * texelSize).r;

				float epsilon = max(EPSILON_FACTOR * (1.0 - normDotFragToLight), EPSILON_MAX);
				if(currentDepth - epsilon > visibleDepth)
				{
					shadow += 1.0;
				}
			}
		}
		shadow /= SHADOW_SAMPLE_RANGE;
	}

	vec3 combined = ambient + (diffuse + specular) * (1.0 - shadow);

	float attenuation = 
		u_Attenuation.constant + 
		dist * u_Attenuation.linear + 
		dist * dist * u_Attenuation.quadratic;

	o_FragColor = vec4(combined / attenuation, 1.0);
}