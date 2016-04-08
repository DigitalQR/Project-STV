#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv_coords;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in float in_texture_id;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

out vec2 pass_uv_coords;
out vec3 pass_normal;

out vec3 texture_ratio;
out float pass_fake_brightness;
flat out float pass_id;


void CalculateFakeLighting()
{
	const vec3 light_direction = normalize(vec3(1,-1,1));
	vec3 unit_normal = normalize(in_normal);

	float normal_dot_light = dot(unit_normal, -light_direction);
	pass_fake_brightness = max(0.2, normal_dot_light);
}


void main()
{
	pass_uv_coords = in_uv_coords;
	pass_normal = in_normal;

	gl_Position = projection_matrix * view_matrix * vec4(in_position, 1.0);

	int tile_track = gl_VertexID;
	while(tile_track >= 3)
	{
		tile_track -=3;
	}

	pass_id = in_texture_id;

	switch(tile_track)
	{
		case 0:
			texture_ratio = vec3(1,0,0);
			break;
		case 1:
			texture_ratio = vec3(0,1,0);
			break;
		case 2:
			texture_ratio = vec3(0,0,1);
			break;
	};

	CalculateFakeLighting();
}