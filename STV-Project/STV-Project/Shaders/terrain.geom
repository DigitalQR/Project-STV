#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;
mat4 vp_matrix = projection_matrix * view_matrix;

uniform vec3 sunlight_direction;

uniform float fog_density;
uniform float fog_gradient;


in VertexData
{
	vec2 uv_coord;
	vec3 normal;
	uint texture_id;
	vec3 to_light;
} vertex_in[];


out VertexDataPass
{
	vec2 uv_coord;
	vec3 normal;
	vec3 texture_ratio;
	float brightness;
	float fog_factor;
	flat vec3 texture_ids;
	vec3 to_light;
} vertex_out;


void CalculateDirectionalBrightness(int index)
{
	vec3 light_direction = normalize(sunlight_direction);
	vec3 unit_normal = normalize(vertex_in[index].normal);

	float normal_dot_light = dot(unit_normal, -light_direction);
	vertex_out.brightness = max(0.2, normal_dot_light);
}


void CalculateFogFactor(int index)
{
	float distance = length((view_matrix * gl_in[index].gl_Position).xyz);
	vertex_out.fog_factor = exp( -pow( distance * fog_density, fog_gradient) );
	clamp(vertex_out.fog_factor, 0.0, 1.0);
}


void main()
{
	for(int i = 0; i < gl_in.length(); i++)
	{
		gl_Position = vp_matrix * gl_in[i].gl_Position;
		vertex_out.uv_coord = vertex_in[i].uv_coord;
		vertex_out.normal = vertex_in[i].normal;
		vertex_out.to_light = vertex_in[i].to_light;
		
		vertex_out.texture_ids.x = vertex_in[0].texture_id;
		vertex_out.texture_ids.y = vertex_in[1].texture_id;
		vertex_out.texture_ids.z = vertex_in[2].texture_id;
		CalculateDirectionalBrightness(i);
		CalculateFogFactor(i);

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