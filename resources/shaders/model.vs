#version 330 core

layout (location = 0) in vec3 position;

out vec3 pix_position;

void main()
{
	pix_position = position;
	gl_Position = vec4(position, 1.0);
}
