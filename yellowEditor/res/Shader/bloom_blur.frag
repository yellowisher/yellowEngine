#version 330 core
in vec2 v_TexCoord;

out vec4 o_FragColor;

uniform sampler2D u_Image;
uniform bool u_Horizontal;

const float u_Weights[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{             
	vec2 unitTex = 1.0 / textureSize(u_Image, 0);
	vec3 result = texture(u_Image, v_TexCoord).rgb * u_Weights[0];

	if(u_Horizontal)
	{
		for(int i = 1; i < 5; i++)
		{
			result += texture(u_Image, v_TexCoord + vec2(unitTex.x * i, 0.0)).rgb * u_Weights[i];
			result += texture(u_Image, v_TexCoord - vec2(unitTex.x * i, 0.0)).rgb * u_Weights[i];
		}
	}
	else
	{
		for(int i = 1; i < 5; i++)
		{
			result += texture(u_Image, v_TexCoord + vec2(0.0, unitTex.y * i)).rgb * u_Weights[i];
			result += texture(u_Image, v_TexCoord - vec2(0.0, unitTex.y * i)).rgb * u_Weights[i];
		}
	}

     o_FragColor = vec4(result, 1.0);
}