#version 450

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_UV;
layout (location = 3) in vec4 a_Color;

layout (location = 0) out vec3 v_WorldPosition;
layout (location = 1) out vec3 v_WorldNormal;
layout (location = 2) out vec3 v_ViewPosition;
layout (location = 3) out vec3 v_ViewNormal;
layout (location = 4) out vec2 v_UV;


layout (std140, binding = 0) uniform Camera
{
    mat4 ViewMatrix;
    mat4 ViewProjectionMatrix;
} ub_Camera;

layout (std140, binding = 3) uniform Model
{
    mat4 Transform;
} ub_Model;


void main()
{   
    v_WorldPosition = (ub_Model.Transform * vec4(a_Position, 1.0)).xyz;
    v_WorldNormal = normalize(mat3(ub_Model.Transform) * vec3(a_Normal)); //REMEMBER TO NOT APPLY NON UNIFORM SCALE IN TRANSFORM MATRIX OR NORMAL VECTORS WILL BE WRONG.

    v_ViewPosition = (ub_Camera.ViewMatrix * vec4(v_WorldPosition, 1.0)).xyz;
    v_ViewNormal = (ub_Camera.ViewMatrix * vec4(v_WorldNormal, 0.0)).xyz;

    v_UV = a_UV;


    gl_Position = ub_Camera.ViewProjectionMatrix * vec4(v_WorldPosition, 1.0);
}