#version 330 core

in vec2 pass_uv_coords;
in vec3 pass_normal;

uniform sampler2D texture0_sampler;

out vec4 colour;

void main()
{
	colour = texture(texture0_sampler, pass_uv_coords);
	
	vec3 normal = pass_normal;
	normal += 1;
	normal /= sqrt(3);

	float bias = 0.85;
	colour.xyz *= bias + normalize(normal)*(1.0-bias)*2;

	if(colour.a == 0)
		discard;



}