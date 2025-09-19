#version 450

layout (location = 0) in vec3 v_WorldPosition;
layout (location = 1) in vec3 v_WorldNormal;
layout (location = 2) in vec3 v_ViewPosition;
layout (location = 3) in vec3 v_ViewNormal;
layout (location = 4) in vec2 v_UV;

layout (location = 0) out vec4 FragColor;

layout(binding = 0) uniform sampler2D Albedo;

void main()
{
    FragColor = vec4(texture(Albedo, v_UV).rgb, 1.0);
}