#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_UV;
layout (location = 3) in vec4 a_Color;

out vec3 v_WorldPosition;
out vec3 v_WorldNormal;

layout (std140, binding = 0) uniform Camera
{
    mat4 ub_ViewProjection;
};

uniform mat4 u_Transform;

void main()
{   
    v_WorldPosition = (u_Transform * vec4(a_Position, 1.0)).xyz;
    v_WorldNormal = normalize(mat3(u_Transform) * vec3(a_Normal)); //REMEMBER TO NOT APPLY NON UNIFORM SCALE IN TRANSFORM MATRIX OR NORMAL VECTORS WILL BE WRONG.

    gl_Position = ub_ViewProjection * vec4(v_WorldPosition, 1.0);
}