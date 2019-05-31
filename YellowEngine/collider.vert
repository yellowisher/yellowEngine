#version 330 core

layout (location = 0) in vec3 a_Position;

uniform mat4 u_Model;
uniform mat4 u_ProjectionView;

void main()
{
	gl_Position = u_ProjectionView * u_Model * vec4(a_Position, 1.0);
}