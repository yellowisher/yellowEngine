#version 330 core

in vec3 a_Position;
in vec3 a_Normal;
in vec2 a_TexCoord0;

out vec3 v_WorldFragPosition;
out vec3 v_Normal;
out vec2 v_TexCoord;
  
uniform mat4 u_Model;
uniform mat4 u_ProjectionView;

void main()
{
	v_WorldFragPosition = vec3(u_Model * vec4(a_Position, 1.0));
	v_Normal            = normalize(u_Model * vec4(a_Normal, 1.0)).xyz;
	v_TexCoord          = a_TexCoord0;
    gl_Position         = u_ProjectionView * u_Model * vec4(a_Position, 1.0);
}