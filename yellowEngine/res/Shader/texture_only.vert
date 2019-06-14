#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_ProjectionView;

void main()
{
	v_TexCoord = a_TexCoord;
    gl_Position = u_ProjectionView * u_Model * vec4(a_Position, 1.0);
}