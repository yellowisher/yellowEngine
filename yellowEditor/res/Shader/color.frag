#version 330 core

in vec3 v_Normal;
in vec3 v_FragPosition;
in vec2 v_TexCoord;

out vec4 o_FragColor;

float near = 0.01;
float far = 100.0;

float LinearzeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
	float depth = LinearzeDepth(gl_FragCoord.z) / far;
    o_FragColor = vec4(depth);
}