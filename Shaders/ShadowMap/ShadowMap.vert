#version 450

layout (location = 0) in vec3 a_Position;

layout (std140, binding = 3) uniform Model
{
    mat4 Transform;
} ub_Model;

layout (std140, binding = 4) uniform LightSpace
{
    mat4 Matrix;
} ub_LightSpace;


void main()
{
    gl_Position = ub_LightSpace.Matrix * ub_Model.Transform * vec4(a_Position, 1.0);
} 