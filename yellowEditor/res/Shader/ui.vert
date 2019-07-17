#version 330

in vec3 a_Position; 
in vec2 a_TexCoord0;

out vec2 v_TexCoord;

uniform float u_WeightX;
uniform float u_BiasX;
uniform float u_WeightY;
uniform float u_BiasY;

void main()
{          
	vec3 position = a_Position;
	position.x = u_WeightX * a_Position.x + u_BiasX;
	position.y = u_WeightY * a_Position.y + u_BiasY;

	v_TexCoord = a_TexCoord0;
    gl_Position = vec4(position, 1.0f);
}
