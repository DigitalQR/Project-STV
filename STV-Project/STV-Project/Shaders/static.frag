#version 330 core

in vec4 pass_colour;

out vec4 colour;

void main()
{
	colour = pass_colour;
}