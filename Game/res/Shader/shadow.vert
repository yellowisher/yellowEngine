#version 330

in vec3 a_Position; 

uniform mat4 u_LightProjView;
uniform mat4 u_Model;

void main()
{          
    gl_Position = u_LightProjView * u_Model * vec4(a_Position, 1.0);
}
