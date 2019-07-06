#version 330

out vec4 o_FragColor;

struct DirLight
{
	vec3 direction;

	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

uniform sampler2D u_PositionMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_ColorMap;
uniform mat4 u_LightProjView;
uniform sampler2D u_ShadowMap;

uniform DirLight u_Light;
uniform vec3 u_CameraPosition;
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


	// Shade
	vec3 ambient = u_Light.color * u_Light.ambientIntensity * color.rgb;

	float normDotFragToLight = max(dot(normal, -u_Light.direction), 0.0);
	float diff = normDotFragToLight;
	vec3 diffuse = u_Light.color * u_Light.diffuseIntensity * color.rgb * diff;

	vec3 fragToCamera = normalize(u_CameraPosition - worldPosition);

	vec3 halfVector = normalize(-u_Light.direction + fragToCamera);
	float spec = clamp(pow(dot(normal, halfVector), 32), 0.0, 1.0);
	vec3 specular = vec3(color.a * spec * u_Light.diffuseIntensity);


	// Shadow
	vec4 lightPosition = u_LightProjView * vec4(worldPosition, 1.0);
	vec3 shadowCoord   = lightPosition.xyz;
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
					// because of light volume, only directional light consider
					// global ambient lighting. So it's fair to reduce shadow about ambient intensity.
					shadow += 0.5; //(1.0 - u_Light.ambientIntensity);
				}
			}
		}
		shadow /= NUM_SHADOW_SAMPLE;
	}

	vec3 combined = ambient + (diffuse + specular) * (1.0 - shadow);

	o_FragColor = vec4(combined, 1.0);
}