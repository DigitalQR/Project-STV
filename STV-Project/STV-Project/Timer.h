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
		return ((unsigned long)clock() - start_time) / (CLOCKS_PER_SEC/1000);
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