#version 330 core

const int MAX_JOINTS = 100;

in vec3 a_Position;
in vec3 a_Normal;
in vec4 a_Joints;
in vec4 a_Weights;

out vec3 v_Normal;
out vec3 v_FragPosition;
  
uniform mat4 u_Model;
uniform mat4 u_ProjectionView;
uniform mat4 u_Joints[MAX_JOINTS];

void main()
{
	mat4 jointMatrix = u_Joints[int(a_Joints[0])] * a_Weights[0];
	jointMatrix += u_Joints[int(a_Joints[1])] * a_Weights[1];
	jointMatrix += u_Joints[int(a_Joints[2])] * a_Weights[2];
	jointMatrix += u_Joints[int(a_Joints[3])] * a_Weights[3];

	if (a_Weights[0] == 0.0)
	{
		jointMatrix = u_Model;
	}

	//mat4 model = u_Model * jointMatrix;

	v_Normal = normalize(mat3(transpose(inverse(jointMatrix))) * a_Normal);
	v_FragPosition = vec3(jointMatrix * vec4(a_Position, 1.0));

    gl_Position = u_ProjectionView * jointMatrix * vec4(a_Position, 1.0);
}