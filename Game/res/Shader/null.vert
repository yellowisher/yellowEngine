#version 330

in vec3 a_Position; 

uniform mat4 u_ProjViewWorld;

void main()
{          
    gl_Position = u_ProjViewWorld * vec4(a_Position, 1.0);
}
