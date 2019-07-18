#version 330 core
out vec4 o_FragColor;

in vec3 v_TexCoord;

uniform samplerCube u_Skybox;

void main()
{    
    o_FragColor = texture(u_Skybox, v_TexCoord);
}