#version 440 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv_coords;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in float in_texture_id;

layout(location = 10) out VertexData
{
	vec2 uv_coord;
	vec3 normal;
	uint texture_id;
	vec3 to_light;
} vertex_out;

uniform vec3 light_location;

void main()
{
	vertex_out.uv_coord = in_uv_coords;
	vertex_out.normal = in_normal;
	vertex_out.texture_id = uint(round(in_texture_id));
	
	gl_Position = vec4(in_position, 1.0);

	vertex_out.to_light = light_location - in_position;
}