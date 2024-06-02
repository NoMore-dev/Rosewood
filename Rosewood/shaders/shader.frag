#version 450 core

out vec4 FragColor;

in vec3 v_Position;

void main()
{
    FragColor = vec4((v_Position+1.0f)/2.0f,1.0f);
}