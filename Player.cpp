#include "Player.h"

void Player::Update(Window& window, float deltaTime)
{
	//Set wallrunning to false
	isWallRunning = false;

	//If the shift button is press
	if (window.keyboard.KeyIsPressed(VK_SHIFT)) {
		//Increase player speed
		playerSpeed = 0.2f;
	}
	else {
		//Reduce player speed
		playerSpeed = 0.1f;
	}
	//If NPC can wall run, and W is pressed, and can move forward
	if (canWallRun && window.keyboard.KeyIsPressed('W') && canMoveForward) {
		//Jumping is false
		isJumping = false;
		//Falling is false
		isFalling = false;
		//Wallrunning is true
		isWallRunning = true;
		//move player forward
		ChangePosition(GetForwardVec(true) * playerSpeed * deltaTime);
	}
	//Else if the player can move forward and w is pressed
	else if (window.keyboard.KeyIsPressed('W') && canMoveForward) {
		//Moveplayer forward
		ChangePosition(GetForwardVec(true) * playerSpeed * deltaTime);
	}
	//If S is pressed
	if (window.keyboard.KeyIsPressed('S') && canMoveBack) {
		//Move player backwards
		ChangePosition(GetBackwardVec(true) * playerSpeed * deltaTime);
	}
	//If A is pressed
	if (window.keyboard.KeyIsPressed('A') && canMoveLeft) {
		//Move player left
		ChangePosition(GetLeftVec(true) * playerSpeed * deltaTime);
	}
	//If D is pressed
	if (window.keyboard.KeyIsPressed('D') && canMoveRight) {
		//Move player right
		ChangePosition(GetRightVec(true) * playerSpeed * deltaTime);
	}
	//If E is pressed
	if (window.keyboard.KeyIsPressed('E') && target != NULL && activateButtonCD == 0) {
		//Interact with object
		///FOR NOW THIS IS SET AS SET FOLLOW PLAYER TRUE. IN THE FUTURE THIS COULD JUST BE INTERACT(), WHICH CAN BE SET TO MANY DIFFERENT THINGS FOR EACH TYPE OF GAMEOBJECT
		target->SetFollowingPlayer(!target->GetFollowingPlayer());
		//Begin cooldown
		activateButtonCD = 60;
	}
	
	//If space is pressed 
	if (window.keyboard.KeyIsPressed(VK_SPACE)) {
		//If not falling or already jumping
		if (!isJumping && !isFalling) {
			//Jump!
			jumpStartHeight = GetPositionFloat3().y;
			isJumping = true;
		}
	}
	//If the player is jumping
	if (isJumping) {
		//if player is lower than max jump height
		if (GetPositionFloat3().y - playerHeight - jumpStartHeight < jumpMaxHeight) {
			//Move the player up
			ChangePosition(0.0f, playerSpeed * deltaTime, 0.0f);
		}
		else {
			//jumping is false
			isJumping = false;
			//Falling is true
			isFalling = true;
		}
	}
	//If the player is not wall running, jumping or touching the floor
	if (!touchingFloor && !isJumping && !isWallRunning) {
		//Fall by gravity
		ChangePosition(0, gravityOnObject * deltaTime, 0);
	}

	//Get the players forward float
	XMFLOAT3 forward;
	XMStoreFloat3(&forward, GetForwardVec());
	//Move the camera to be slightly infront of the player
	camera.SetPosition(GetPositionFloat3().x + forward.x, GetPositionFloat3().y + playerHeight, GetPositionFloat3().z + forward.z);
	SetRotation(0, camera.GetRotationFloat3().y, 0);
	//Create players collision box
	CreateAABBCC();

	//Reduce the cooldown on the activate button
	if (activateButtonCD > 0) {
		activateButtonCD--;
	}
}


