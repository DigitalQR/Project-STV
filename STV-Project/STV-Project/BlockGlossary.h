#pragma once

enum resource_id {
	RES_AIR = 2500, RES_UNKNOWN,
	RES_GRASS = 0, RES_DIRT, RES_STONE
};

struct Resource
{
	resource_id resource;
	float density = 0;

	Resource() 
	{
		resource = RES_AIR;
	}

	Resource(resource_id resource) : resource(resource) {};
	
	void operator=(resource_id res)
	{
		resource = res;
		density = 0;
	}

	bool operator==(resource_id res)
	{
		return resource == res;
	}

	bool operator!=(resource_id res)
	{
		return resource != res;
	}
};

static bool IsSolid(resource_id res)
{
	return res != RES_AIR;
}

static bool IsSolid(Resource res)
{
	return res != RES_AIR;
}
