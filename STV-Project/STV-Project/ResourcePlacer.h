#pragma once
#include "Component.h"
#include "Camera.h"
#include "Mouse.h"
#include "GameManager.h"
#include "Terrain.h"
#include <vector>


using namespace std;


class ResourcePlacer : public Component
{
private:
	Terrain* terrain;

	bool last_u_state = false;
	bool HasPlaced()
	{
		const bool key = Mouse::isButtonDown(1);
		if (key != last_u_state)
		{
			last_u_state = key;
			return key;
		}
		return false;
	}

	bool last_o_state = false;
	bool HasDestroyed()
	{
		const bool key = Mouse::isButtonDown(0);
		if (key != last_o_state)
		{
			last_o_state = key;
			return key;
		}
		return false;
	}
	
	bool last_mode_state = false;
	unsigned int place_type = 0;

	void CheckModeToggle()
	{
		const bool key = Mouse::isButtonDown(2);
		if (key != last_mode_state)
		{
			last_mode_state = key;
			if (key)
			{
				place_type++;
				if (place_type > 2)
					place_type = 0;
			}
		}
	}

	
	void UpdateResouce() 
	{
		for (int i = (int)RES_DIRT; i <= (int)RES_PLANKS; i++)
		{
			if (Keyboard::isKeyDown(48 + i)) 
			{
				current_resource = (resource_id)i;
				break;
			}
		}
	}

	resource_id current_resource = RES_DIRT;

public:

	void Start()
	{
		terrain = GameManager::getMain()->GetCurrentScene()->GetTerrain();
	}

	void VisualUpdate()
	{
		CheckModeToggle();

		bool placed = HasPlaced();
		bool destroyed = HasDestroyed();

		UpdateResouce();

		if (!placed && !destroyed)
			return;

		vec3 direction = parent->getForward() * (1.5f + place_type);
		direction.x = direction.x;
		direction.y = direction.y;
		direction.z = direction.z;

		const vec3 loc = parent->location + vec3(0, 0.3f, 0) + direction;
		const Vectori location(floor(loc.x), floor(loc.y), floor(loc.z));

		vector<Vectori> cube
		{
			Vectori(location.x, location.y, location.z),
			Vectori(location.x + 1, location.y, location.z),
			Vectori(location.x + 1, location.y, location.z + 1),
			Vectori(location.x, location.y, location.z + 1),

			Vectori(location.x, location.y + 1, location.z),
			Vectori(location.x + 1, location.y + 1, location.z),
			Vectori(location.x + 1, location.y + 1, location.z + 1),
			Vectori(location.x, location.y + 1, location.z + 1),
		};

		vector<Vectori> sphere
		{
			Vectori(location.x, location.y, location.z),


			Vectori(location.x + 1, location.y, location.z),
			Vectori(location.x - 1, location.y, location.z),

			Vectori(location.x, location.y + 1, location.z),
			Vectori(location.x, location.y - 1, location.z),

			Vectori(location.x, location.y, location.z + 1),
			Vectori(location.x, location.y, location.z - 1),


			Vectori(location.x - 1, location.y - 1, location.z),
			Vectori(location.x + 1, location.y - 1, location.z),
			Vectori(location.x - 1, location.y + 1, location.z),
			Vectori(location.x + 1, location.y + 1, location.z),

			Vectori(location.x - 1, location.y, location.z - 1),
			Vectori(location.x + 1, location.y, location.z - 1),
			Vectori(location.x - 1, location.y, location.z + 1),
			Vectori(location.x + 1, location.y, location.z + 1),

			Vectori(location.x, location.y - 1, location.z - 1),
			Vectori(location.x, location.y + 1, location.z - 1),
			Vectori(location.x, location.y - 1, location.z + 1),
			Vectori(location.x, location.y + 1, location.z + 1),
		};

		if (placed)
		{

			if (place_type == 0)
			{
				terrain->PlaceResource(location.x, location.y, location.z, RES_BRICKS, false);
			}
			else if (place_type == 1)
			{
				terrain->PlaceResources(cube, current_resource, false);
			}
			else
			{
				terrain->PlaceResources(sphere, current_resource, false);
			}
		}
		else if (destroyed)
		{
			if (place_type == 0)
			{
				terrain->PlaceResource(location.x, location.y, location.z, RES_AIR, true);
			}
			else if (place_type == 1)
			{
				terrain->PlaceResources(cube, RES_AIR, true);
			}
			else
			{
				terrain->PlaceResources(sphere, RES_AIR, true);
			}
		}
	};

	void LogicUpdate() {}

};