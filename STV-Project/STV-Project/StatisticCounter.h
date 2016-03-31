#pragma once
#include <thread>
#include "Timer.h"
#include <iostream>

class StatisticCounter 
{
public:
	void StartCounter() 
	{
		thread counter(&StatisticCounter::Counter, this);
		running = true;
		counter.detach();
	}

	void StopCounter() 
	{
		running = false;
	}

	void Increment() 
	{
		count++;
	}

	unsigned int GetCount() 
	{
		unsigned int temp = count;
		count = 0;
		return temp;
	}

	bool isRunning() 
	{
		return running;
	}

private:
	unsigned int count = 0;
	Timer timer;
	bool running = false;

	void Counter() 
	{
		std::cout << "Starting counter" << endl;
		while (running)
		{
			timer.start();
			timer.HoldUntilExceeded(1000);
			std::cout << "UPS: " << GetCount() << endl;
		}
	}
};