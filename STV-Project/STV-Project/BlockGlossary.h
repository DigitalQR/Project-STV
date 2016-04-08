#pragma once

enum resource_id {
	RES_AIR = 2500, RES_UNKNOWN,
	RES_GRASS = 0, RES_DIRT, RES_STONE
};

static bool IsSolid(resource_id res) 
{
	return res != RES_AIR;
}
