#version 330

in vec2 v_TexCoord;

out vec4 o_FragColor;

uniform sampler2D u_Image;

void main()
{
	o_FragColor = texture(u_Image, v_TexCoord);
}