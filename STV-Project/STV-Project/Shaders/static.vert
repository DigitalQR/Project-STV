#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv_coords;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

out vec2 pass_uv_coords;

void main()
{
	pass_uv_coords = in_uv_coords;

	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(in_position, 1.0);
}