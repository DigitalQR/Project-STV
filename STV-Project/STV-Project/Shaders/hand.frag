#version 330 core

in vec2 pass_uv_coords;
in vec3 pass_normal;
in vec3 to_light;

const int HORIZONTAL_IDS = 8;
const int VERTICAL_IDS = 8;
uniform sampler2D texture0_sampler;

uniform vec3 sunlight_direction;
uniform vec3 sunlight_colour;

uniform vec3 light_colour;

uniform float item_id;

out vec4 colour;


vec3 CalculateDirectionalBrightness()
{
	vec3 light_direction = normalize(sunlight_direction);
	vec3 unit_normal = normalize(pass_normal);

	float normal_dot_light = dot(unit_normal, -light_direction);
	float brightness = max(0.2, normal_dot_light);

	return brightness * sunlight_colour;
}


void CalculateLighting()
{
	vec3 total_diffuse = CalculateDirectionalBrightness();

	vec3 unit_to_light = normalize(to_light);
	float distance_to_light = length(to_light);

	vec3 unit_normal = normalize(pass_normal);
	float normal_dot_light = dot(unit_normal, unit_to_light);
	float brightness = max(normal_dot_light, 0.0);

	total_diffuse += (brightness * light_colour);

	colour.xyz *= total_diffuse;
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


void main()
{
	colour = GetTexture(uint(item_id), CorrectUVs(pass_uv_coords));
	
	if(colour.a == 0)
		discard;
	
	CalculateLighting();
}