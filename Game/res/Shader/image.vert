#version 330

in vec3 a_Position; 
in vec2 a_TexCoord0;

out vec2 v_TexCoord;

uniform vec2 u_Weight;
uniform vec2 u_Bias;

void main()
{          
	vec3 position = a_Position;
	position.x = u_Weight.x * a_Position.x + u_Bias.x;
	position.y = u_Weight.y * a_Position.y + u_Bias.y;

	v_TexCoord = a_TexCoord0;
    gl_Position = vec4(position, 1.0f);
}
