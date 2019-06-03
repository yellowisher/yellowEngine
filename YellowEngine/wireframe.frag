#version 330 core

out vec4 o_FragColor;

uniform vec3 u_LineColor;

void main()
{
    o_FragColor = vec4(u_LineColor, 1.0);
}