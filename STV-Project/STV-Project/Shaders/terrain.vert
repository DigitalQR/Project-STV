#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv_coords;
layout(location = 2) in vec3 in_normal;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

out vec2 pass_uv_coords;
out vec3 pass_normal;

out vec3 texture_ratio;
flat out int pass_id;

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
		
	switch(tile_track)
	{
		case 0:
			texture_ratio = vec3(1,0,0);
			pass_id = 1;
			break;
		case 1:
			texture_ratio = vec3(0,1,0);
			pass_id = 1;
			break;
		case 2:
			texture_ratio = vec3(0,0,1);
			pass_id = 1;
			break;
	};
	
}