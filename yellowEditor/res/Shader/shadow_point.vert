#version 330

in vec3 a_Position; 

out vec4 v_WorldFragPosition;

uniform mat4 u_LightProjView;
uniform mat4 u_Model;

void main()
{          
	v_WorldFragPosition = u_Model * vec4(a_Position, 1.0);
    gl_Position =  u_LightProjView * v_WorldFragPosition;
}
