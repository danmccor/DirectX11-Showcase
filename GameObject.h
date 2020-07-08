#pragma once
#include "Transform.h"
#include "Window.h"
#include "CollisionManager.h"
#include "Timer.h"

class GameObject : public Transform
{
public:

	//Initialise the game object
	bool Initialise(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<vs_cBuffer>& vs_ConstantBuffer, bool isStatic = false);
	//Set the game objects transform
	void SetTransform(XMFLOAT3 position = XMFLOAT3(0,0,0), XMFLOAT3 scale = XMFLOAT3(1,1,1), XMFLOAT3 rotation = XMFLOAT3(0, 0, 0));
	//Draw the object
	void Draw(const XMMATRIX& viewProjectionMatrix);
	//Update the object (this will be applied to all game objects
	virtual void Update(Window& window, float deltaTime);
	//Overloaded update for NPCs
	virtual void Update(Window& window, GameObject& player, CollisionManager collisionManager, float deltaTime);
	
	//Return the gameobjects minimum and maximum bounding box
	XMVECTOR& GetMinBoundingBox() { return minBoundingBox; };
	XMVECTOR& GetMaxBoundingBox() { return maxBoundingBox; };

	//Set the minimum and maximum bounding box
	void SetMinBoundingBox(XMVECTOR minBound) { minBoundingBox = minBound; };
	void SetMaxBoundingBox(XMVECTOR maxBound) {maxBoundingBox = maxBound; };

	//Get is Static (stops the collision box position being recalculated
	bool GetIsStatic() { return isStatic; };
	//Get is lit (Changes which shader to use on the object)
	bool GetIsLit() { return isLit; };

	//Create the bounding box
	void CreateAABBCC(float xChangeMax = 0, float yChangeMax = 0, float zChangeMax = 0, float xChangeMin = 0, float yChangeMin = 0, float zChangeMin = 0);

	//Setters and getter for variables used by game objects (mainly NPC's and players)
	void IsJumping(bool isJumping);
	void IsFalling(bool isFalling);
	void IsTouchingFloor(bool touchingFloor);
	void SetFollowingPlayer(bool following) { followingPlayer = following; };
	bool GetFollowingPlayer(bool following) { return following; };
	void CanMoveForward(bool canMoveForward);
	void CanMoveBack(bool canMoveBack);
	void CanMoveRight(bool canMoveRight);
	void CanMoveLeft(bool canMoveLeft);
	void CanWallRun(bool canWallRun) { this->canWallRun = canWallRun; };
	void Jump() { isJumping = true; };
	int GetHealth() { return health; };
	bool GetHasTarget() { if (target != NULL) return true; else return false; };
	void SetTarget(GameObject* gameObject);

	//Used to change the NPC's actions
	bool GetFollowingPlayer() {return followingPlayer;};

protected:
	//Set model for the NPC
	Model model;

	//CollisionManager collisionManager;
	XMVECTOR minBoundingBox = { 0,0,0,0 };
	XMVECTOR maxBoundingBox = { 0,0,0,0 };

	//Variables for the GameObject
	bool isStatic = false;
	bool isLit = true;

	bool isJumping = false;
	bool isFalling = false;
	bool touchingFloor = false;

	bool canWallRun = false;
	bool isWallRunning = false;
	bool canMoveForward = true;
	bool canMoveBack = true;
	bool canMoveRight = true;
	bool canMoveLeft = true;
	bool followingPlayer = false;

	float jumpStartHeight = 0;
	float jumpMaxHeight = 20.0f;
	float height = 14.0f;

	int health = 100;

	GameObject* target = nullptr;

	float gravityOnObject = -0.2f;
};

