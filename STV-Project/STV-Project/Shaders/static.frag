#version 330 core

in vec2 pass_uv_coords;
in vec3 pass_normal;

uniform sampler2D texture0_sampler;

out vec4 colour;

void main()
{
	colour = texture(texture0_sampler, pass_uv_coords);
	
	float normal = abs(pass_normal.x + pass_normal.y + pass_normal.z)/3;

	float bias = 0.8;
	colour.xyz *= bias + normal*(1.0-bias)*2;

	if(colour.a == 0)
		discard;

}