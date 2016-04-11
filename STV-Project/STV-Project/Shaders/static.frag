#version 330 core

in vec2 pass_uv_coords;
in vec3 pass_normal;
in float fog_factor;

uniform sampler2D texture0_sampler;

uniform vec3 fog_colour;

out vec4 colour;

void UseFakeLighting()
{
	const vec3 light_direction = normalize(vec3(1,-1,1));
	vec3 unit_normal = normalize(pass_normal);

	float normal_dot_light = dot(unit_normal, -light_direction);
	float brightness = max(0.2, normal_dot_light);
	colour *= brightness;
}

void main()
{
	colour = texture(texture0_sampler, pass_uv_coords);
	if(colour.a == 0)
		discard;
	
	UseFakeLighting();
	colour.xyz = mix(fog_colour, colour.xyz, fog_factor);
}