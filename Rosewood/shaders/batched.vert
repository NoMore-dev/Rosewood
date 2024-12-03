#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_UV;
layout (location = 3) in vec4 a_Color;

out vec3 v_WorldPosition;
out vec3 v_WorldNormal;


uniform mat4 u_ViewProjection;

layout(std430, binding = 0) buffer transformBuffer
{
    mat4 transforms[];
};


void main()
{   
    int id = gl_DrawID;

    v_WorldPosition = (transforms[id] * vec4(a_Position, 1.0)).xyz;
    v_WorldNormal = normalize(mat3(transforms[id]) * vec3(a_Normal)); //REMEMBER TO NOT APPLY NON UNIFORM SCALE IN TRANSFORM MATRIX OR NORMAL VECTORS WILL BE WRONG.

    gl_Position = u_ViewProjection * vec4(v_WorldPosition, 1.0);
}
