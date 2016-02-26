#version 330 core

in vec2 pass_uv_coords;

uniform sampler2D texture0_sampler;

out vec4 colour;

void main()
{
	colour = texture(texture0_sampler, pass_uv_coords);

	if(colour.a == 0)
		discard;
}