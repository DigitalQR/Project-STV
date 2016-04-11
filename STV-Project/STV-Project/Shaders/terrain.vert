#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv_coords;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in float in_texture_id;

out VertexData
{
	vec2 uv_coord;
	vec3 normal;
	uint texture_id;
} vertex_out;


void main()
{
	vertex_out.uv_coord = in_uv_coords;
	vertex_out.normal = in_normal;
	vertex_out.texture_id = uint(round(in_texture_id));
	gl_Position = vec4(in_position, 1.0);
}