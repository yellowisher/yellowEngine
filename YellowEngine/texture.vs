#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 projectionView;

void main()
{
	//gl_Position = vec4(aPosition, 1.0);
    gl_Position = projectionView * model * vec4(aPosition, 1.0);
    TexCoord = aTexCoord;
}