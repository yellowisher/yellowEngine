#version 330 core
in vec3 a_Position;

out vec2 v_TexCoord;

void main()
{
    v_TexCoord = (a_Position.xy + vec2(1)) * 0.5;;
    gl_Position = vec4(a_Position, 1.0);
}