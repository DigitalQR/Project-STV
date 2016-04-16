#pragma once
#include "Dependencies\glm\common.hpp"


using namespace glm;


struct Light 
{
	vec3 location;
	vec3 colour;
	vec3 attenuation;

	Light()
	{
		location = vec3(0, 0, 0);
		colour = vec3(0, 0, 0);
		attenuation = vec3(1, 0, 0);
	}

	Light(vec3 location) : location(location)
	{
		colour = vec3(0, 0, 0);
		attenuation = vec3(1, 0, 0);
	}

	Light(vec3 location, vec3 colour) : location(location), colour(colour)
	{
		attenuation = vec3(1, 0, 0);
	}

	Light(vec3 location, vec3 colour, vec3 attenuation) : location(location), colour(colour), attenuation(attenuation)
	{
	}

	void operator=(Light& b) 
	{
		location = b.location;
		colour = b.colour;
		attenuation = b.attenuation;
	}
};