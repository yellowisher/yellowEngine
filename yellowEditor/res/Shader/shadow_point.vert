#version 330

const int MAX_JOINTS = 100;

in vec3 a_Position; 
in vec4 a_Joints;
in vec4 a_Weights;

out vec4 v_WorldFragPosition;

uniform mat4 u_LightProjView;
uniform bool u_Skinning;
uniform mat4 u_Model;
uniform mat4 u_Joints[MAX_JOINTS];

void main()
{          
	mat4 model = u_Model;
	if(u_Skinning)
	{
		mat4 jointMatrix = u_Joints[int(a_Joints[0])] * a_Weights[0];
		jointMatrix += u_Joints[int(a_Joints[1])] * a_Weights[1];
		jointMatrix += u_Joints[int(a_Joints[2])] * a_Weights[2];
		jointMatrix += u_Joints[int(a_Joints[3])] * a_Weights[3];
		model = model * jointMatrix;
	}

	v_WorldFragPosition = model * vec4(a_Position, 1.0);
    gl_Position =  u_LightProjView * v_WorldFragPosition;
}
