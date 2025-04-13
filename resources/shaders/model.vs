#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;


uniform mat4 SV_MODEL_MAT;
uniform mat4 SV_VIEW_MAT;
uniform mat4 SV_PROJ_MAT;

out vec3 pix_position;
out vec3 pix_normal;

void main()
{
    mat4 mvp = SV_PROJ_MAT * SV_VIEW_MAT * SV_MODEL_MAT;
    vec4 pos = mvp * vec4(position.xyz, 1.0);
    pix_position = pos.xyz;
    pix_normal = normal;

    gl_Position = pos;
}
