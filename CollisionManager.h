#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
class CollisionManager
{
public:
	//Declare types of collision checking
	bool CheckCollision(DirectX::XMVECTOR& obj1MinVert, XMVECTOR& obj1MaxVert, XMVECTOR& obj2MinVert, XMVECTOR& obj2MaxVert);
	bool RayCastCollision(DirectX::XMVECTOR ray, float rayLegnth, int rayDirection, XMMATRIX worldPos, XMVECTOR& objMinVert, XMVECTOR& objMaxVert);
};

