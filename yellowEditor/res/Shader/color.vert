#version 330 core

in vec3 a_Position;
in vec3 a_Normal;

out vec3 v_Normal;
out vec3 v_FragPosition;
  
uniform mat4 u_Model;
uniform mat4 u_ProjectionView;
uniform 

void main()
{
	v_Normal = normalize(mat3(transpose(inverse(u_Model))) * a_Normal);
	v_FragPosition = vec3(u_Model * vec4(a_Position, 1.0));

    gl_Position = u_ProjectionView * u_Model * vec4(a_Position, 1.0);
}