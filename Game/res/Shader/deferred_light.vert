#version 330 

in vec3 a_Position; 

out vec2 v_TexCoord;

uniform mat4 u_ProjViewWorld;

void main()
{ 
   	gl_Position = u_ProjViewWorld * vec4(a_Position, 1.0);
	v_TexCoord = (a_Position.xy + vec2(1)) * 0.5;
} 