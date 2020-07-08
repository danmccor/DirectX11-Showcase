#include "NPC.h"


void NPC::Update(Window& window, GameObject& player, CollisionManager collisionManager, float deltaTime)
{
	//Subtract the players position from our position
	XMVECTOR direction = XMVectorSubtract(GetPositionVector(), player.GetPositionVector());
	//Normalise the direction
	direction = XMVector3Normalize(direction);

	//If the npc is not touching the floor
	if (!touchingFloor) {
		//And the npc is not jumping or falling
		if (!isJumping && !isFalling) {
			//Jump!
			jumpStartHeight = GetPositionFloat3().y;
			Jump();
		}
	}
	//If the npc is jumping
	if (isJumping) {
		//If their current height is less than the maximum height
		if (GetPositionFloat3().y - height - jumpStartHeight < jumpMaxHeight) {
			//Move up
			ChangePosition(0.0f, npcSpeed * deltaTime, 0.0f);
		}
		//Else begin falling
		else {
			isJumping = false;
			isFalling = true;
		}
	}
	//Look at the player
	setLookAtPosition(player.GetPositionFloat3());

	//If the NPC can move forward
	if (canMoveForward) {
		//Store the direction in a temp float
		///you cannot access the vector variables
		XMFLOAT3 tmp;
		XMStoreFloat3(&tmp, direction);
		//Move the npc forward by its speed
		tmp.x *= (npcSpeed * deltaTime);
		tmp.y = 0;
		tmp.z *= (npcSpeed * deltaTime);
		//Change its position
		ChangePosition(-tmp.x, tmp.y, -tmp.z);
	}

}
