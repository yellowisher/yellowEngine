#version 330

in vec2 v_TexCoord;

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

void main()
{
	vec3 worldPosition = texture(u_PositionMap, v_TexCoord).xyz;
	vec4 color         = texture(u_ColorMap, v_TexCoord);
	vec3 normal        = normalize(texture(u_NormalMap, v_TexCoord).xyz);

	vec3 ambient = u_Light.color * u_Light.ambientIntensity * color.rgb;

	float diff = max(dot(normal, -u_Light.direction), 0.0);
	vec3 diffuse = u_Light.color * u_Light.diffuseIntensity * color.rgb * diff;

	vec3 fragToCamera = normalize(u_CameraPosition - worldPosition);
	vec3 halfVector = normalize(-u_Light.direction + fragToCamera);
	float spec = clamp(pow(dot(normal, halfVector), 32), 0.0, 1.0);
	vec3 specular = vec3(color.a * spec * u_Light.diffuseIntensity);

	// shadow
	float e = 0.005;

	vec4 lightPosition = u_LightProjView * vec4(worldPosition, 1.0);
	vec3 lightProjPos  = lightPosition.xyz / lightPosition.w;
	lightProjPos = lightProjPos * 0.5 + 0.5;

	float visibleDepth = texture(u_ShadowMap, lightProjPos.xy).r;

	float currentDepth = lightProjPos.z;
	float shadow = ((currentDepth - e) < visibleDepth) ? 1.0 : 0;

	vec3 combined = ambient + (diffuse + specular) * shadow;

	o_FragColor = vec4(combined, 1.0);
}