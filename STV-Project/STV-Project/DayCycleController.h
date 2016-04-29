#pragma once
#include "Component.h"
#include "GameManager.h"

#define PI 3.14159265359

struct TimeProfile 
{
public:
	const vec3 colour;
	const vec3 sky_colour;
	const float start;
	const float end;
	TimeProfile* next;

	TimeProfile(float start, float end, vec3 colour, vec3 sky_colour) :
		start(start), end(end), colour(colour), sky_colour(sky_colour)
	{
		LERP_start = start + (end-start) * 0.5f;
	}

	void SetColours(float time) 
	{
		if (time > LERP_start) 
		{
			float lerp = (time - LERP_start) / (end - LERP_start);
			vec3 colour = this->colour * (1.0f - lerp) + next->colour * lerp;
			vec3 sky_colour = this->sky_colour * (1.0f - lerp) + next->sky_colour * lerp;

			GameManager::getMain()->sunlight_colour = colour;
			GameManager::getMain()->sky_colour = sky_colour;

			vec3 fog = sky_colour * 1.0f;
			if (fog.x > 1)
				fog.x = 1;
			if (fog.y > 1)
				fog.y = 1;
			if (fog.z > 1)
				fog.z = 1;

			GameManager::getMain()->fog_colour = fog;
		}
		else
		{
			GameManager::getMain()->sunlight_colour = colour;
			GameManager::getMain()->sky_colour = sky_colour;

			vec3 fog = sky_colour * 1.05f;
			if (fog.x > 1)
				fog.x = 1;
			if (fog.y > 1)
				fog.y = 1;
			if (fog.z > 1)
				fog.z = 1;

			GameManager::getMain()->fog_colour = fog;
		}
	}
private:
	float LERP_start;
};

class DayCycleController : public Component 
{
public:
	float time_scale = 1;

	DayCycleController() 
	{
		sunrise->next = day;
		day->next = sunset;
		sunset->next = moonrise;
		moonrise->next = night;
		night->next = sunrise;
	}

	~DayCycleController() 
	{
		for (TimeProfile* time_profile : day_times)
			delete time_profile;
	}

	void Start() 
	{

	}

	void VisualUpdate() {}

	void LogicUpdate() 
	{
		GameManager::getMain()->sunlight_direction = vec3(cos(_time_track), -0.5, sin(_time_track));
		_time_track += _time_step * time_scale;

		if (_time_track >= 2 * PI)
			_time_track -= 2 * PI;

		float day_time = GetDayTime();
		
		for (TimeProfile* time_profile: day_times)
			if (day_time >= time_profile->start && day_time < time_profile->end) 
			{
				time_profile->SetColours(day_time);
				break;
			}
	}

	float GetDayTime() 
	{
		return _time_track / (2 * PI);
	}

private:
	const float _cycle_duration_mins = 5.0f;
	const float _time_step = ((2 * PI) * 0.033333333f) / (_cycle_duration_mins * 60.0f);
	float _time_track = 0;

	TimeProfile* sunrise = new TimeProfile(0.0f, 0.1f, vec3(1.0f, 0.79f, 0.05f), vec3(1.0f, 0.79f, 0.05f));
	TimeProfile* day = new TimeProfile(0.1f, 0.45f, vec3(1.0f, 1.0f, 1.0f), vec3(0.53f, 0.81f, 0.98f));
	TimeProfile* sunset = new TimeProfile(0.45f, 0.55f, vec3(0.89f, 0.38f, 0.4f), vec3(1.0f, 0.79f, 0.05f));
	TimeProfile* moonrise = new TimeProfile(0.55f, 0.65f, vec3(0.38f, 0.1f, 0.89f), vec3(0.3f, 0.1f, 0.6f));
	TimeProfile* night = new TimeProfile(0.65f, 1.0f, vec3(0.38f, 0.2f, 0.99f), vec3(0.1f, 0.1f, 0.5f));

	array<TimeProfile*, 5> day_times{ sunrise, day, sunset, moonrise, night };
	
};