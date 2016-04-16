#version 330 core

in VertexDataPass
{
	vec2 uv_coord;
	vec3 normal;
	vec3 texture_ratio;
	float brightness;
	float fog_factor;
	flat vec3 texture_ids;
	vec3 to_light;
} vertex_data;


const int HORIZONTAL_IDS = 8;
const int VERTICAL_IDS = 8;
uniform sampler2D texture0_sampler;

uniform vec3 fog_colour;
uniform vec3 sunlight_colour;

uniform vec3 light_colour;
uniform vec3 light_attenuation;

out vec4 colour;

vec2 CorrectUVs(vec2 uvs)
{
	vec2 corrected_uvs = uvs;
	const float cushion = 0.001;

	//Correct UV scale
	while(corrected_uvs.x >= 1)
		corrected_uvs.x--;
	while(corrected_uvs.y >= 1)
		corrected_uvs.y--;
		
	if(corrected_uvs.x < cushion)
		corrected_uvs.x = cushion;
	if(corrected_uvs.x > 1 - cushion)
		corrected_uvs.x = 1 - cushion;

	if(corrected_uvs.y < cushion)
		corrected_uvs.y = cushion;
	if(corrected_uvs.y > 1 - cushion)
		corrected_uvs.y = 1 - cushion;

	return corrected_uvs;
}


vec4 GetTexture(uint id, vec2 uv_coords)
{
	vec2 step = vec2(
		1.0/HORIZONTAL_IDS,
		1.0/VERTICAL_IDS
	);

	
	uv_coords.x/=HORIZONTAL_IDS;
	uv_coords.y/=VERTICAL_IDS;
	
	uv_coords.x += step.x * mod(id, HORIZONTAL_IDS);
	uv_coords.y += step.y * floor(float(id)/VERTICAL_IDS);
	//Texture is upsidedown
	uv_coords.y = 1.0 - uv_coords.y;
	
	return texture(texture0_sampler, uv_coords);
}

void CalculateLighting()
{
	vec3 total_diffuse = vertex_data.brightness * sunlight_colour;

	vec3 unit_to_light = normalize(vertex_data.to_light);
	float distance_to_light = length(vertex_data.to_light);

	vec3 unit_normal = normalize(vertex_data.normal);
	float normal_dot_light = dot(unit_normal, unit_to_light);
	float brightness = max(normal_dot_light, 0.0);

	float attenuation_factor = light_attenuation.x + light_attenuation.y * distance_to_light + light_attenuation.z * distance_to_light * distance_to_light;

	total_diffuse += (brightness * light_colour)/attenuation_factor;

	colour.xyz *= total_diffuse;
}


void main()
{
	vec2 uvs = CorrectUVs(vertex_data.uv_coord);

	colour = 
		GetTexture(uint(vertex_data.texture_ids.x), uvs) * vertex_data.texture_ratio.x +
		GetTexture(uint(vertex_data.texture_ids.y), uvs) * vertex_data.texture_ratio.y +
		GetTexture(uint(vertex_data.texture_ids.z), uvs) * vertex_data.texture_ratio.z;

	if(colour.a == 0)
		discard;

	CalculateLighting();
	colour.xyz = mix(fog_colour, colour.xyz, vertex_data.fog_factor);
}