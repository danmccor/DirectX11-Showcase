#pragma once
#include <chrono>
//Timer class for keeping everything the same speed
class Timer
{
public:
	//timer Constructor
	Timer();
	//Get the amount of miliseconds since timer started
	double GetMiliSeconds();
	//Restart the timer
	void Restart();
	//Stop the timer
	bool Stop();
	//Start the timer
	bool Start();

private:
	//variables for the timer
	bool isRunning = false;
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> stop;
};

