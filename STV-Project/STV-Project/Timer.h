#pragma once
#include <time.h>

class Timer
{
public:
	void start()
	{
		start_time = clock();
	}

	unsigned long GetElapsedTime()
	{
		return ((unsigned long)clock() - start_time) / ((float)CLOCKS_PER_SEC / 1000.0f);
	}

	float GetElapsedTimef()
	{
		return ((float)clock() - (float)start_time) / ((float)CLOCKS_PER_SEC / 1000.0f);
	}

	bool HasExceeded(float time_ms)
	{
		return (GetElapsedTime() > time_ms);
	}

	void HoldUntilExceeded(float sleep_ms)
	{
		bool throw_away = 0;

		while (true)
		{
			if (GetElapsedTime() > sleep_ms)
			{
				break;
			}
			else
			{
				throw_away = 0;
			}
		}
	}


private:
	unsigned long start_time;


};