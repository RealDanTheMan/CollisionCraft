#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 pix_position;
out vec3 pix_normal;

void main()
{
	pix_position = position;
	pix_normal = normal;
	gl_Position = vec4(position, 1.0);
}
