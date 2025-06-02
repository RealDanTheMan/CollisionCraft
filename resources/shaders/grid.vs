#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 SV_VIEW_MAT;
uniform mat4 SV_PROJ_MAT;


void main()
{
    mat4 mvp = SV_PROJ_MAT * SV_VIEW_MAT;
    vec4 pos = mvp * vec4(position.xyz, 1.0);

    gl_Position = pos;
}
