#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 SV_VIEW_MAT;
uniform mat4 SV_PROJ_MAT;

out vec3 pix_position;
out vec3 cam_position;


void main()
{
    mat4 mvp = SV_PROJ_MAT * SV_VIEW_MAT;
    vec4 pos = mvp * vec4(position.xyz, 1.0);
    pix_position = pos.xyz;
    cam_position = inverse(SV_VIEW_MAT)[3].xyz;

    gl_Position = pos;
}
