#pragma once
#pragma once
#include "Component.h"
#include "Camera.h"
#include "Keyboard.h"
#include "GameManager.h"
#include "Terrain.h"
#include <vector>


using namespace std;


class ResourcePlacer : public Component
{
public:
	Terrain* terrain;

	void Start()
	{
		terrain = GameManager::getMain()->GetCurrentScene()->GetTerrain();
	}

	bool last_u_state = false;
	bool HasPlaced()
	{
		const bool key = Keyboard::isKeyDown('u');
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
		const bool key = Keyboard::isKeyDown('o');
		if (key != last_o_state)
		{
			last_o_state = key;
			return key;
		}
		return false;
	}

	void VisualUpdate()
	{
		if (HasPlaced())
		{
			vec3 direction = parent->getForward() * 1.5f;
			direction.x = floor(direction.x);
			direction.y = floor(direction.y);
			direction.z = floor(direction.z);

			const vec3 loc = parent->location + direction + vec3(0,0.5f,0);
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

			terrain->PlaceResources(vec, RES_DIRT, false);
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

			terrain->PlaceResources(vec, RES_AIR, true);
		}
	};

	void LogicUpdate() {};

};