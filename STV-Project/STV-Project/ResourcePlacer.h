#pragma once
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
	bool place_mode = true;

	void CheckModeToggle()
	{
		const bool key = Mouse::isButtonDown(2);
		if (key != last_mode_state)
		{
			last_mode_state = key;
			if(key)
				place_mode = !place_mode;
		}
	}

public:

	void Start()
	{
		terrain = GameManager::getMain()->GetCurrentScene()->GetTerrain();
	}

	void VisualUpdate()
	{
		CheckModeToggle();

		if (HasPlaced())
		{
			vec3 direction = parent->getForward() * 2.0f;
			direction.x = round(direction.x);
			direction.y = round(direction.y);
			direction.z = round(direction.z);

			const vec3 loc = parent->location + direction;
			const Vectori location(round(loc.x), round(loc.y), round(loc.z));
			vector<Vectori> vec
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

			if (place_mode)
			{
				terrain->PlaceResources(vec, RES_DIRT, false);
			}
			else 
			{
				terrain->PlaceResource(location.x, location.y, location.z, RES_DIRT, false);
			}
		}
		else if (HasDestroyed())
		{
			vec3 direction = parent->getForward() * 1.5f;
			direction.x = floor(direction.x);
			direction.y = floor(direction.y);
			direction.z = floor(direction.z);

			const vec3 loc = parent->location + direction + vec3(0, 0.5f, 0);
			const Vectori location(round(loc.x), round(loc.y), round(loc.z));
			vector<Vectori> vec
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
						
			if (place_mode)
			{
				terrain->PlaceResources(vec, RES_AIR, true);
			}
			else
			{
				terrain->PlaceResource(location.x, location.y, location.z, RES_AIR, true);
			}
		}
	};

	void LogicUpdate() {};

};