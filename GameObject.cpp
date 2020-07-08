#include "GameObject.h"

//Initialise the game object
bool GameObject::Initialise(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<vs_cBuffer>& vs_ConstantBuffer, bool isStatic)
{
	//If does not get initialised, return false
	if (!model.Initialise(filePath, device, deviceContext, vs_ConstantBuffer)) return false;
	//Set the position, rotation and scale of the object
	SetPosition(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f, 0.0f, 0.0f);
	SetScale(1.0f, 1.0f, 1.0f);
	//Update its world matrix
	this->UpdateWorldMatrix();
	//Return true
	return true;
}

//Set the transform of the object
void GameObject::SetTransform(XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation)
{
	//Set position, scale and rotation
	SetPosition(position);
	SetScale(scale);
	SetRotation(rotation);
	//Update the collision box
	CreateAABBCC();
}

//Draw the game object
void GameObject::Draw(const XMMATRIX& viewProjectionMatrix)
{
	//Draw the model
	model.Draw(worldMat, viewProjectionMatrix);
}

//Update the game object, unless overrided, this is the default behaviour of a game object
void GameObject::Update(Window& window, float deltaTime)
{
	//If the object is not touching the floor
	if (!touchingFloor && !isJumping) {
		//Apply gravity
		ChangePosition(0, gravityOnObject * deltaTime, 0);
	}
	//If the objects is not static
	if (!isStatic) {
		//Update its collision box
		CreateAABBCC();
	}
}

//Here so child classes can override it.
void GameObject::Update(Window& window, GameObject& player, CollisionManager collisionManager, float deltaTime)
{
}


//Create the collision box. Input has default values so that if called with nothing, it will just create the collision box as standard
///These are here so we can modify the collision. For example with the buildings there are vertices higher than the roof, we pass in -200 on max Y to set the collision box as lower
void GameObject::CreateAABBCC(float xChangeMax, float yChangeMax, float zChangeMax, float xChangeMin, float yChangeMin, float zChangeMin)
{
	//Min and max vertices are initialised
	XMFLOAT3 minVertex = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	XMFLOAT3 maxVertex = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	//For each side of the bounding box
	for (UINT i = 0; i < model.GetBoundingBox().size(); i++) {
		//Vector is set the bounding box
		XMVECTOR vert = XMVectorSet(model.GetBoundingBox()[i].x, model.GetBoundingBox()[i].y, model.GetBoundingBox()[i].z, 0.0f);
		//The vertex is transformed this this game objects world position
		vert = XMVector3TransformCoord(vert, worldMat);

		//We find the minumum and maxiumum on all axis
		minVertex.x = min(minVertex.x, XMVectorGetX(vert));
		minVertex.y = min(minVertex.y, XMVectorGetY(vert));
		minVertex.z = min(minVertex.z, XMVectorGetZ(vert));

		maxVertex.x = max(maxVertex.x, XMVectorGetX(vert));
		maxVertex.y = max(maxVertex.y, XMVectorGetY(vert));
		maxVertex.z = max(maxVertex.z, XMVectorGetZ(vert));
	}
	//We minus or plus the amount we want to change the collision
	minVertex.x += xChangeMin;
	minVertex.y += yChangeMin;
	minVertex.z += zChangeMin;

	maxVertex.x += xChangeMax;
	maxVertex.y += yChangeMax;
	maxVertex.z += zChangeMax;

	//We set the bounding box
	minBoundingBox = XMVectorSet(minVertex.x, minVertex.y, minVertex.z, 0.0f);
	maxBoundingBox = XMVectorSet(maxVertex.x, maxVertex.y, maxVertex.z, 0.0f);
}


//Setters
///Normally would do this in the header file, was for some reason not accepting these values existed
void GameObject::IsJumping(bool isJumping)
{
	this->isJumping = isJumping;
}

void GameObject::IsFalling(bool isFalling)
{
	this->isFalling = isFalling;
}

void GameObject::IsTouchingFloor(bool touchingFloor)
{
	this->touchingFloor = touchingFloor;
}

void GameObject::CanMoveForward(bool canMoveForward)
{
	this->canMoveForward = canMoveForward;
}

void GameObject::CanMoveBack(bool canMoveBack)
{
	this->canMoveBack = canMoveBack;
}

void GameObject::CanMoveRight(bool canMoveRight)
{
	this->canMoveRight = canMoveRight;
}

void GameObject::CanMoveLeft(bool canMoveLeft)
{
	this->canMoveLeft = canMoveLeft;
}

void GameObject::SetTarget(GameObject* gameObject)
{
	target = gameObject;
}

