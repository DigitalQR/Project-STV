#version 330 core

in vec2 pass_uv_coords;
in vec3 pass_normal;
in float fog_factor;
in vec3 to_light;

uniform sampler2D texture0_sampler;

uniform vec3 fog_colour;
uniform vec3 sunlight_direction;
uniform vec3 sunlight_colour;

uniform vec3 light_colour;
uniform vec3 light_attenuation;

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

	float attenuation_factor = light_attenuation.x + light_attenuation.y * distance_to_light + light_attenuation.z * distance_to_light * distance_to_light;

	total_diffuse += (brightness * light_colour)/attenuation_factor;

	colour.xyz *= total_diffuse;
}

void main()
{
	colour = texture(texture0_sampler, pass_uv_coords);
	if(colour.a == 0)
		discard;
	
	CalculateLighting();
	colour.xyz = mix(fog_colour, colour.xyz, fog_factor);
}