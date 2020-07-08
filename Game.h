#pragma once
#include "Window.h"
#include "Graphics.h"
#include "CollisionManager.h"
#include "Timer.h"
#include <string>
#include "NPC.h"
#include "Player.h"
#include "Light.h"

class Game
{
	//Set public methods
public: 
	//Initialise the game
	void Initialise(HINSTANCE hInst);
	//Update the game
	void Update();
	//Manage Windows Messages
	bool manageMessages();
	//Render the frame
	void Render();

private:
	//Create private variables
	Window window;
	Graphics graphics;
	Timer timer;
	CollisionManager collisionManager;

	//Set the nullptr for player, npcs, and light.
	Player* player = nullptr;
	NPC* npc = nullptr;
	Light* light = nullptr;

	//Create a vector to hold the text that will go onto the screen and the game objects in the scene
	std::vector<std::string> strings;
	std::vector<GameObject*> gameObjects;
	
};

