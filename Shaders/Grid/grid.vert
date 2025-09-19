#version 450

layout (location = 0) in vec3 a_Position;

layout (location = 0) out vec3 v_WorldPosition;


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

    gl_Position = ub_Camera.ViewProjectionMatrix * vec4(v_WorldPosition, 1.0);
}