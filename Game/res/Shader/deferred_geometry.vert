#version 330 core

const int MAX_JOINTS = 100;

in vec3 a_Position;
in vec3 a_Normal;
in vec2 a_TexCoord0;
in vec4 a_Joints;
in vec4 a_Weights;
in vec3 a_Tangent;
in vec3 a_Bitangent;

out vec3 v_WorldFragPosition;
out vec2 v_TexCoord;
out vec3 v_Tangent;
out vec3 v_Bitangent;
out vec3 v_Normal;
  
uniform bool u_Skinning;
uniform mat4 u_Model;
uniform mat4 u_ProjectionView;
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

	v_WorldFragPosition = vec3(model * vec4(a_Position, 1.0));
	v_TexCoord          = a_TexCoord0;
	v_Tangent			= normalize(vec3(model * vec4(a_Tangent,	0)));
	v_Bitangent			= normalize(vec3(model * vec4(a_Bitangent,	0)));
	v_Normal			= normalize(vec3(model * vec4(a_Normal,		0)));

    gl_Position         = u_ProjectionView * model * vec4(a_Position, 1.0);
}