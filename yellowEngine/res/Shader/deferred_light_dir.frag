#version 330

out vec4 o_FragColor;

struct DirLight
{
	vec3 direction;

	vec3 color;
	float ambiendIntensity;
	float diffuseIntensity;
};

uniform sampler2D u_PositionMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_ColorMap;

uniform vec2 u_ScreenSize;
uniform DirLight u_Light;
uniform vec3 u_CameraPosition;

void main()
{
    vec2 texCoord      = gl_FragCoord.xy / u_ScreenSize;
	vec3 worldPosition = texture(u_PositionMap, texCoord).xyz;
	vec4 color         = texture(u_ColorMap, texCoord);
	vec3 normal        = normalize(texture(u_NormalMap, texCoord).xyz);

	vec3 ambient = u_Light.color * u_Light.ambiendIntensity * color.rgb;

	float diff = max(dot(normal, -u_Light.direction), 0.0);
	vec3 diffuse = u_Light.color * u_Light.diffuseIntensity * color.rgb * diff;

	vec3 fragToCamera = normalize(u_CameraPosition - worldPosition);
	vec3 halfVector = normalize(-u_Light.direction + fragToCamera);
	float spec = clamp(pow(dot(normal, halfVector), 32), 0.0, 1.0);
	vec3 specular = vec3(color.a * spec);

	vec3 combined = ambient + diffuse + specular;

	o_FragColor = vec4(combined, 1.0);
}