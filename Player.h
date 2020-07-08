#pragma once
#include "GameObject.h"
#include "Camera.h"

class Player : public GameObject
{
public:
	//Create a camera for the player
	Camera camera;
	//Update the player (overriding the gameobject update)
	void Update(Window& window, float deltaTime);
	
private:
	//Set the players height
	float playerHeight = 14.0f;
	//Set the players speed
	float playerSpeed = 0.1f;

	//Variables for activate cooldown
	bool lastFrameActivateKey = false;
	int activateButtonCD = 0;
};

