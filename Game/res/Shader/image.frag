#version 330

in vec2 v_TexCoord;

out vec4 o_FragColor;

uniform sampler2D u_Image;
uniform vec3 u_Color;

void main()
{
	o_FragColor = texture(u_Image, v_TexCoord) * vec4(u_Color, 1.0);
}