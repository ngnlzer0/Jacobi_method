#pragma once

#include<chrono>

using namespace std::chrono;

/**
 * The simple class which implements a simple timer
 */
class Timer
{
private:
	time_point<steady_clock> time;
	bool isRunning;

public:
	void start()
	{
		time = high_resolution_clock().now();
		isRunning = true;
	}

	microseconds stop()
	{
		if (isRunning)
		{
			isRunning = false;
			return duration_cast<microseconds>(high_resolution_clock().now() - time);
		}
		else
		{
			return duration_cast<microseconds>(time.time_since_epoch());
		}
	}


};