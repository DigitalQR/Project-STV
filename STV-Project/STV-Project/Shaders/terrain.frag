#version 330 core

in vec2 pass_uv_coords;
in vec3 pass_normal;

in vec3 texture_ratio;
flat in int pass_id;

const int HORIZONTAL_IDS = 8;
const int VERTICAL_IDS = 8;

uniform sampler2D texture0_sampler;

out vec4 colour;

void UseFakeLighting()
{
	const vec3 light_direction = normalize(vec3(1,-1,1));
	vec3 unit_normal = normalize(pass_normal);

	float normal_dot_light = dot(unit_normal, -light_direction);
	float brightness = max(0.2, normal_dot_light);
	colour *= brightness;
}

//Texture is upsidedown
vec4 GetTexture(int id, vec2 uv_coords)
{
	vec2 step = vec2(
		1.0/HORIZONTAL_IDS,
		1.0/VERTICAL_IDS
	);

	
	uv_coords.x/=HORIZONTAL_IDS;
	uv_coords.y/=VERTICAL_IDS;
	
	uv_coords.x += step.x * mod(id, HORIZONTAL_IDS);
	uv_coords.y += step.y * floor(float(id/VERTICAL_IDS));
	uv_coords.y = 1.0 - uv_coords.y;
	
	return texture(texture0_sampler, uv_coords);
}

void main()
{
	vec2 corrected_uvs = pass_uv_coords;
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

	vec3 correct_ratio = normalize(texture_ratio);

	colour = GetTexture(pass_id, corrected_uvs) * correct_ratio.x +
	GetTexture(pass_id, corrected_uvs) * correct_ratio.y +
	GetTexture(pass_id, corrected_uvs) * correct_ratio.z;
	if(colour.a == 0)
		discard;
	
	UseFakeLighting();
}