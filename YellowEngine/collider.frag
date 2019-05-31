#version 330 core

out vec4 o_FragColor;

uniform vec3 u_ColliderColor;

void main()
{
    o_FragColor = vec4(0, 0.8, 0, 1.0);
    //o_FragColor = vec4(u_ColliderColor, 1.0);
}