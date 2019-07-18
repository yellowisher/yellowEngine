#version 330 core
in vec3 a_Position;

out vec3 v_TexCoord;

uniform mat4 u_ProjectionView;

void main()
{
    v_TexCoord = a_Position;
    gl_Position = u_ProjectionView * vec4(a_Position, 1.0);
}  