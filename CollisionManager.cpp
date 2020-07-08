#include "CollisionManager.h"

bool CollisionManager::CheckCollision(DirectX::XMVECTOR& obj1MinVert, XMVECTOR& obj1MaxVert, XMVECTOR& obj2MinVert, XMVECTOR& obj2MaxVert)
{
	//Standard AABB(CC) Collision Detection

	///If the maximum X of object 1 is greater than the minumum X of Object 2
	if (XMVectorGetX(obj1MaxVert) > XMVectorGetX(obj2MinVert)) {
		///If the minimum X of object 1 is less than the maximum X of Object 2
		if (XMVectorGetX(obj1MinVert) < XMVectorGetX(obj2MaxVert)) {
			///If the maximum Y of object 1 is greater than the minumum Y of Object 2
			if (XMVectorGetY(obj1MaxVert) > XMVectorGetY(obj2MinVert)) {
				///If the minimum Y of object 1 is less than the maximum Y of Object 2
				if (XMVectorGetY(obj1MinVert) < XMVectorGetY(obj2MaxVert)) {
					///If the maximum Z of object 1 is greater than the minumum Z of Object 2
					if (XMVectorGetZ(obj1MaxVert) > XMVectorGetZ(obj2MinVert)) {
						///If the minimum Z of object 1 is less than the maximum Z of Object 2
						if (XMVectorGetZ(obj1MinVert) < XMVectorGetZ(obj2MaxVert)) {
							//The objects are colliding
							return true;
						}
					}
				}
			}
		}
	}
	//The objects are not colliding
	return false;
}

bool CollisionManager::RayCastCollision(DirectX::XMVECTOR ray, float rayLength, int rayDirection, XMMATRIX worldPos, XMVECTOR& objMinVert, XMVECTOR& objMaxVert)
{
	/*
	RAY DIRECTIONS:
		0 = FORWARD
		1 = BACKWARD
		2 = RIGHT
		3 = LEFT 
		4 = UP
		5 = DOWN
	*/

	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, ray);
	float rayHeightModifier = 2.0f;
	float rayOffset = 10.0f;

	switch (rayDirection)
	{
	case 0: //FORWARD
		tmp.y += rayHeightModifier;
		tmp.z += rayLength;
		break;
	case 1: //BACKWARD
		tmp.y += rayHeightModifier;
		tmp.z -= rayLength;
		break; 
	case 2: //RIGHT
		tmp.y += rayHeightModifier;
		tmp.x += rayLength;
		break;
	case 3: //LEFT
		tmp.y += rayHeightModifier;
		tmp.x -= rayLength;
		break;
	case 4: //UP
		tmp.y += (rayOffset + rayLength);
		break;
	case 5: //DOWN
		tmp.y -= rayLength;
		break;
	default: 
		break;
	}
	ray = XMLoadFloat3(&tmp);
	//tmp.x += rayLength;
	ray = XMVector3TransformCoord(ray, worldPos);
	//Similar to the AABB Collision above, however this time the ray is a single point to compare between all Vertices
	if (XMVectorGetX(ray) > XMVectorGetX(objMinVert)) {
		if (XMVectorGetX(ray) < XMVectorGetX(objMaxVert)) {
			if (XMVectorGetY(ray) > XMVectorGetY(objMinVert)) {
				if (XMVectorGetY(ray) < XMVectorGetY(objMaxVert)) {
					if (XMVectorGetZ(ray) > XMVectorGetZ(objMinVert)) {
						if (XMVectorGetZ(ray) < XMVectorGetZ(objMaxVert)) {
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}
