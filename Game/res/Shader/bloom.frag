#version 330 core

out vec4 o_FragColor;

in vec2 v_TexCoord;

uniform sampler2D u_Scene;
uniform sampler2D u_Bloom;
uniform float u_Exposure;
uniform bool u_ApplyBloom;

const float gamma = 2.2;

void main()
{
	vec3 scene = texture(u_Scene, v_TexCoord).rgb;
	vec3 bloom = texture(u_Bloom, v_TexCoord).rgb;
	if(u_ApplyBloom)
	{
		scene += bloom;
	}

	vec3 result = vec3(1.0, 1.0, 1.0) - exp(-scene * u_Exposure);

	result = pow(result, vec3(1.0 / gamma));
	o_FragColor = vec4(result, 1.0);
}