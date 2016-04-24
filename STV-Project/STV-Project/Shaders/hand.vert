#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv_coords;
layout(location = 2) in vec3 in_normal;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

out vec2 pass_uv_coords;
out vec3 pass_normal;
out vec3 to_light;

void main()
{
	pass_uv_coords = in_uv_coords;

	pass_normal = (model_matrix * vec4(in_normal, 0.0)).xyz;
	vec4 world_position = model_matrix * vec4(in_position, 1.0);

	gl_Position = projection_matrix * view_matrix * world_position;

	to_light = vec3(0,0,0) - world_position.xyz;
}