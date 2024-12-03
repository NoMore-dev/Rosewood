#type vertex

#version 450 core

layout (location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;
layout(location = 3) in vec3 a_Color;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_UV;

void main()
{
    v_UV = a_UV;
    
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}



#type fragment

#version 450 core

out vec4 FragColor;

in vec2 v_UV;

uniform sampler2D u_Texture;

void main()
{
    FragColor = vec4(vec3(1.0), texture(u_Texture, v_UV).r);
}