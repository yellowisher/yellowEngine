#version 330

in vec4 v_WorldFragPosition;

out float o_FragColor;

uniform vec3 u_LightPosition;

void main()
{
    float dist = length(v_WorldFragPosition.xyz - u_LightPosition);
	o_FragColor = dist;
}