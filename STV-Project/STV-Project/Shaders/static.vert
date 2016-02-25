#version 330 core

layout(location = 0) in vec3 in_position;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

out vec4 pass_colour;

void main()
{
	gl_Position = projection_matrix * view_matrix * vec4(in_position, 1.0);
	pass_colour = vec4(in_position, 1.0);
}