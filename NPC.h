#pragma once
#include "GameObject.h"
#include "Player.h"
class NPC : public GameObject
{
public: 
	//Update for when following the player
	void Update(Window& window, GameObject& player, CollisionManager collisionManager, float deltaTime);

private:
	//Set the npc speed and its gravity
	float npcSpeed = 0.1f;
};

