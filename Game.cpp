#include "Game.h"

void Game::Initialise(HINSTANCE hInst)
{
	//Start the game Timer
	timer.Start();
	HRESULT hr = (CoInitialize(NULL));
	if (FAILED(hr)) return;

	//Initialise the window
	if (!window.InitialiseWindow(hInst, "Daniel McCormick - Assignment", "WindowClass", 1800, 1080)) {
		return;
	}
	//Initialise the graphics
	if (!graphics.Initialise(window.GetHandle())) {
		return;
	}
	//Create the player
	player = new Player();
	player->Initialise("res/nanosuit/nanosuit2.obj", graphics.device, graphics.deviceContext, graphics.vs_constantBuffer);
	player->camera.SetProjectionValues(70.0f, static_cast<float>(graphics.GetWindowWidth()) / static_cast<float>(graphics.GetWindowHeight()), 0.1f, 10000.0f);
	player->SetPosition(0, 991, 0);
	//Polymorphic push back
	gameObjects.push_back(player);

	//Create an NPC (This could be done as a loop later on, just here for testing purposes
	npc = new NPC();
	npc->Initialise("res/nanosuit/nanosuit2.obj", graphics.device, graphics.deviceContext, graphics.vs_constantBuffer);
	npc->SetPosition(10, 991, 10);
	npc->SetRotation(0, 180, 0);
	//Polymorphic push back
	gameObjects.push_back(npc);

	light = new Light();
	light->Initialise(graphics.device, graphics.deviceContext, graphics.vs_constantBuffer);
	//polymorphic push back
	gameObjects.push_back(light);

}

void Game::Update()
{
	//Delta time = miliseconds since last frame
	float deltaTime = (float)timer.GetMiliSeconds();
	//Restart the timer
	timer.Restart();

	//Clear the text to strings
	strings.clear();
	//Set strings to display on screen
	std::string playerHealth = "Player Health: " + std::to_string(player->GetHealth());
	strings.push_back(playerHealth);
	std::string playerHeight = "Player Y Pos: " + std::to_string(player->GetPositionFloat3().y);
	strings.push_back(playerHeight);
	std::string playerHasTarget = "NPC in View: " + std::to_string(player->GetHasTarget());
	strings.push_back(playerHasTarget);
	
	//Check if there has been any input from keyboard
	while (!window.keyboard.CharBufferIsEmpty()) {
		unsigned char ch = window.keyboard.ReadChar();
	}
	while (!window.keyboard.KeyBufferIsEmpty()) {
		KeyboardEvent kbe = window.keyboard.ReadKey();
		unsigned char ch = kbe.GetKeyCode();
	}
	//Check if there has been any input from mouse
	while (!window.mouse.EventBufferIsEmpty()) {
		MouseEvent me = window.mouse.ReadEvent();
		if (me.GetType() == MouseEvent::EventType::RAW_MOVE) {
			if (window.mouse.IsRightDown()) {
				player->camera.ChangeRotation((float)me.GetPosY() * 0.01f, (float)me.GetPosX() * 0.01f, 0);
			}
		}
	}


	//GAME OBJECT COLLISION WITH BUILDINGS
	for(int i = 0; i < gameObjects.size(); i++){
		//Reset movements to true, this will be changed to false before movement if the player is still trying to walk into a wall
		gameObjects[i]->CanMoveForward(true);
		gameObjects[i]->CanMoveBack(true);
		gameObjects[i]->CanMoveRight(true);
		gameObjects[i]->CanMoveLeft(true);
		gameObjects[i]->IsTouchingFloor(false);
		gameObjects[i]->CanWallRun(false);
		for (int j = 0; j < graphics.map.gameObjects.size(); j++) {
			if (collisionManager.RayCastCollision(gameObjects[i]->GetRightVec(), 3, 0, gameObjects[i]->GetWorldMatrix(), graphics.map.gameObjects[j].GetMinBoundingBox(), graphics.map.gameObjects[j].GetMaxBoundingBox())) {
				//forward
				gameObjects[i]->CanMoveForward(false);
			}
			if (collisionManager.RayCastCollision(gameObjects[i]->GetRightVec(), 3, 1, gameObjects[i]->GetWorldMatrix(), graphics.map.gameObjects[j].GetMinBoundingBox(), graphics.map.gameObjects[j].GetMaxBoundingBox())) {
				//back
				gameObjects[i]->CanMoveBack(false);
			}
			if (collisionManager.RayCastCollision(gameObjects[i]->GetRightVec(), 3, 2, gameObjects[i]->GetWorldMatrix(), graphics.map.gameObjects[j].GetMinBoundingBox(), graphics.map.gameObjects[j].GetMaxBoundingBox())) {
				//right
				gameObjects[i]->CanMoveRight(false);
				gameObjects[i]->CanWallRun(true);
			}
			if (collisionManager.RayCastCollision(gameObjects[i]->GetRightVec(), 3, 3, gameObjects[i]->GetWorldMatrix(), graphics.map.gameObjects[j].GetMinBoundingBox(), graphics.map.gameObjects[j].GetMaxBoundingBox())) {
				//left
				gameObjects[i]->CanMoveLeft(false);
				gameObjects[i]->CanWallRun(true);
			}
			if (collisionManager.RayCastCollision(gameObjects[i]->GetForwardVec(), 1, 5, gameObjects[i]->GetWorldMatrix(), graphics.map.gameObjects[j].GetMinBoundingBox(), graphics.map.gameObjects[j].GetMaxBoundingBox())) {
				//Down
				gameObjects[i]->IsTouchingFloor(true);
				gameObjects[i]->IsJumping(false);
				gameObjects[i]->IsFalling(false);
			}
		}
	}
	//GAME OBJECTS COLLISION WITH OTHER GAMEOBJECTS
	for (int i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->SetTarget(NULL);
		//We dont need to reset the movements here, as if they are trying to talk into a wall it will be false anyway, and if they aren't it will remain true
		for (int j = 0; j < gameObjects.size(); j++) {
			if (gameObjects[i] != gameObjects[j]) {
				if (collisionManager.RayCastCollision(gameObjects[i]->GetRightVec(), 3, 0, gameObjects[i]->GetWorldMatrix(), gameObjects[j]->GetMinBoundingBox(), gameObjects[j]->GetMaxBoundingBox())) {
					//forward
					gameObjects[i]->CanMoveForward(false);
					gameObjects[i]->SetTarget(gameObjects[j]);
				}
				if (collisionManager.RayCastCollision(gameObjects[i]->GetRightVec(), 3, 1, gameObjects[i]->GetWorldMatrix(), gameObjects[j]->GetMinBoundingBox(), gameObjects[j]->GetMaxBoundingBox())) {
					//back
					gameObjects[i]->CanMoveBack(false);
				}
				if (collisionManager.RayCastCollision(gameObjects[i]->GetRightVec(), 3, 2, gameObjects[i]->GetWorldMatrix(), gameObjects[j]->GetMinBoundingBox(), gameObjects[j]->GetMaxBoundingBox())) {
					//right
					gameObjects[i]->CanMoveRight(false);
				}
				if (collisionManager.RayCastCollision(gameObjects[i]->GetRightVec(), 3, 3, gameObjects[i]->GetWorldMatrix(), gameObjects[j]->GetMinBoundingBox(), gameObjects[j]->GetMaxBoundingBox())) {
					//left
					gameObjects[i]->CanMoveLeft(false);
				}
				if (collisionManager.RayCastCollision(gameObjects[i]->GetForwardVec(), 3, 5, gameObjects[i]->GetWorldMatrix(), gameObjects[j]->GetMinBoundingBox(), gameObjects[j]->GetMaxBoundingBox())) {
					//Down
					gameObjects[i]->IsTouchingFloor(true);
					gameObjects[i]->IsJumping(false);
					gameObjects[i]->IsFalling(false);
				}
			}
		}
	}


	if (window.keyboard.KeyIsPressed('R')) {
		//Reset player and NPC Position to start
		player->SetPosition(0, 991, 0);
		npc->SetPosition(10, 991, 10);
	}
	
	for (int i = 0; i < gameObjects.size(); i++) {
		//For each game object in the scene update them
		gameObjects[i]->Update(window, deltaTime);
		if (gameObjects[i]->GetFollowingPlayer()) {
			//Use different update if npc is following player
			gameObjects[i]->Update(window, *player, collisionManager, deltaTime);
		}
	}

	if (window.keyboard.KeyIsPressed('F')) {
		//If F is pressed, bring the light to the player
		XMVECTOR lightPosition = player->camera.GetPositionVector();
		lightPosition += player->camera.GetForwardVec();
		graphics.light.SetPosition(lightPosition);
		graphics.light.SetRotation(player->camera.GetRotationFloat3());
	}
}

//Manage the window messages
bool Game::manageMessages()
{
	//If windows is still managing messages, return true
	if (window.ManageMessages()) {
		return true;
	}
	return false;

}

//Render the frame
void Game::Render()
{
	graphics.RenderFrame(gameObjects, strings, player->camera);
}
