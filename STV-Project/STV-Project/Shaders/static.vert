#version 330 core

layout(location = 0) in vec3 in_position;

out vec4 pass_colour;

void main()
{
	gl_Position = vec4(in_position, 1.0);
	pass_colour = vec4(in_position, 1.0);
}