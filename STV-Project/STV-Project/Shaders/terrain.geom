#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;
mat4 vp_matrix = projection_matrix * view_matrix;

in VertexData
{
	vec2 uv_coord;
	vec3 normal;
	uint texture_id;
} vertex_in[];

out VertexDataPass
{
	vec2 uv_coord;
	vec3 normal;
	vec3 texture_ratio;
	float brightness;
	flat vec3 texture_ids;
} vertex_out;


void CalculateFakeBrightness(int index)
{
	const vec3 light_direction = normalize(vec3(1,-1,1));
	vec3 unit_normal = normalize(vertex_in[index].normal);

	float normal_dot_light = dot(unit_normal, -light_direction);
	vertex_out.brightness = max(0.2, normal_dot_light);
}


void main()
{
	for(int i = 0; i < gl_in.length(); i++)
	{
		gl_Position = vp_matrix * gl_in[i].gl_Position;
		vertex_out.uv_coord = vertex_in[i].uv_coord;
		vertex_out.normal = vertex_in[i].normal;
		
		vertex_out.texture_ids.x = vertex_in[0].texture_id;
		vertex_out.texture_ids.y = vertex_in[1].texture_id;
		vertex_out.texture_ids.z = vertex_in[2].texture_id;
		CalculateFakeBrightness(i);

		switch(int(mod(i,3)))
		{
			case 0:
				vertex_out.texture_ratio = vec3(1,0,0);
				break;
			case 1:
				vertex_out.texture_ratio = vec3(0,1,0);
				break;
			case 2:
				vertex_out.texture_ratio = vec3(0,0,1);
				break;
		};

		EmitVertex();
	}
	EndPrimitive();
}