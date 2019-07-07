#version 330 core
in vec3 a_Position;

out vec2 v_TexCoord;

void main()
{             
	v_TexCoord = a_Position.xy * 0.5 + vec2(0.5);
	gl_Position = vec4(a_Position, 1.0);
}