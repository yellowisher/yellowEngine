#version 330 core

in vec2 v_TexCoord;

out vec4 o_FragColor;

uniform sampler2D u_Diffuse;

void main()
{    
	o_FragColor = texture(u_Diffuse, v_TexCoord);
}