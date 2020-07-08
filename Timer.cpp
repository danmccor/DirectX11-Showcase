#include "Timer.h"
//Timer constructor
Timer::Timer()
{
	//Start and stop time default to now
	start = std::chrono::high_resolution_clock::now();
	stop = std::chrono::high_resolution_clock::now();
}

//Get milisecond past since timer has started
double Timer::GetMiliSeconds()
{
	//if it is running 
	if (isRunning) {
		//Return the time now - when the timer started
		auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start);
		return elapsed.count();
	}
	//if it is not running
	else {
		//Return the stop time - the start time
		auto elapsed = std::chrono::duration<double, std::milli>(stop - start);
		return elapsed.count();
	}
}

void Timer::Restart()
{
	//running = true
	isRunning = true;
	//Start time = now
	start = std::chrono::high_resolution_clock::now();
}

bool Timer::Stop()
{
	//If timer is not running return false
	if (!isRunning) {
		return false;
	}
	else
	{
		//Else stop time = now
		stop = std::chrono::high_resolution_clock::now();
		isRunning = false;
		return true;
	}
}

bool Timer::Start()
{
	//If it is running
	if (isRunning)
	{
		//Return false
		return false;
	}
	else
	{
		//Start time = now
		start = std::chrono::high_resolution_clock::now();
		isRunning = true;
		return true;
	}
}
