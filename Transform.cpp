#include "Transform.h"

//Update the world matrix
void Transform::UpdateWorldMatrix()
{
	//World mat = rot * pos * scale
	worldMat = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * XMMatrixTranslation(pos.x, pos.y, pos.z) * XMMatrixScaling(scale.x, scale.y, scale.z);
	//Change direction vectors with new world mat
	UpdateDirectionVectors();
}

void Transform::UpdateDirectionVectors()
{
	//vector Rotation matrix is set to the rotation of the object
	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	//All sides are transformed by the rotation
	forward = XMVector3TransformCoord(forwardVec, vecRotationMatrix);
	backward = XMVector3TransformCoord(backVec, vecRotationMatrix);
	left = XMVector3TransformCoord(leftVec, vecRotationMatrix);
	right = XMVector3TransformCoord(rightVec, vecRotationMatrix);

	//Set the no Y's so that the vectors do not rotate up or down
	XMMATRIX vecRotationMatrixNoY = XMMatrixRotationRollPitchYaw(0.0f, rot.y, 0.0f);
	forward_noY = XMVector3TransformCoord(forwardVec, vecRotationMatrixNoY);
	backward_noY = XMVector3TransformCoord(backVec, vecRotationMatrixNoY);
	left_noY = XMVector3TransformCoord(leftVec, vecRotationMatrixNoY);
	right_noY = XMVector3TransformCoord(rightVec, vecRotationMatrixNoY);
}

//Constructor for transform
Transform::Transform()
{
	//initialise values
	 forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	 left = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	 right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	 backward = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);

	 forward_noY = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	 left_noY = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	 right_noY = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	 backward_noY = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
}

//Set the position with a vector
void Transform::SetPosition(const XMVECTOR& pos)
{
	XMStoreFloat3(&this->pos, pos);
	posVector = pos;
	UpdateWorldMatrix();
}

//Set the position with a float 3
void Transform::SetPosition(const XMFLOAT3& pos)
{
	this->pos = pos;
	posVector = XMLoadFloat3(&pos);
	UpdateWorldMatrix();
}

//Set the position with 3 different floats
void Transform::SetPosition(float x, float y, float z)
{
	pos = XMFLOAT3(x, y, z);
	posVector = XMLoadFloat3(&pos);
	UpdateWorldMatrix();
}

//Change the position with a vector
void Transform::ChangePosition(const XMVECTOR& pos)
{
	posVector += pos;
	XMStoreFloat3(&this->pos, posVector);
	UpdateWorldMatrix();
}

//Change the position with a float3
void Transform::ChangePosition(const XMFLOAT3& pos)
{
	this->pos.x += pos.x;
	this->pos.y += pos.y;
	this->pos.z += pos.z;
	posVector = XMLoadFloat3(&pos);
	UpdateWorldMatrix();
}

//Change the position with 3 different floats
void Transform::ChangePosition(float x, float y, float z)
{
	pos.x += x;
	pos.y += y;
	pos.z += z;
	posVector = XMLoadFloat3(&pos);
	UpdateWorldMatrix();
}





//Set the rotation with a vector
void Transform::SetRotation(const XMVECTOR& rot)
{
	rotVector = rot;
	XMStoreFloat3(&this->rot, rot);
	UpdateWorldMatrix();
}
//Set the rotation with a float 3
void Transform::SetRotation(const XMFLOAT3& rot)
{
	this->rot = rot;
	rotVector = XMLoadFloat3(&this->rot);
	UpdateWorldMatrix();
}
//Set the rotation with 3 different floats
void Transform::SetRotation(float x, float y, float z)
{
	this->rot = XMFLOAT3(x, y, z);
	rotVector = XMLoadFloat3(&this->rot);
	UpdateWorldMatrix();
}

//Change the rotation with a vector
void Transform::ChangeRotation(const XMVECTOR& rot)
{
	rotVector += rot;
	XMStoreFloat3(&this->rot, rotVector);
	UpdateWorldMatrix();
}

//Change the rotation with a float3
void Transform::ChangeRotation(const XMFLOAT3& rot)
{
	this->rot.x += rot.x;
	this->rot.y += rot.y;
	this->rot.z += rot.z;
	rotVector = XMLoadFloat3(&rot);
	UpdateWorldMatrix();
}

//Change the rotation with 3 different floats
void Transform::ChangeRotation(float x, float y, float z)
{
	rot.x += x;
	rot.y += y;
	rot.z += z;
	rotVector = XMLoadFloat3(&rot);
	UpdateWorldMatrix();
}


//Set the scale with a vector
void Transform::SetScale(const XMVECTOR& scale)
{
	XMStoreFloat3(&this->scale, scale);
	scaleVector = scale;
	UpdateWorldMatrix();
}

//Set the scale with a float3
void Transform::SetScale(const XMFLOAT3& scale)
{
	this->scale = scale;
	scaleVector = XMLoadFloat3(&scale);
	UpdateWorldMatrix();
}

//Set the scale with 3 different floats
void Transform::SetScale(float x, float y, float z)
{
	scale = XMFLOAT3(x, y, z);
	scaleVector = XMLoadFloat3(&scale);
	UpdateWorldMatrix();
}

//Change the scale with a vector
void Transform::ChangeScale(const XMVECTOR& scale)
{
	scaleVector += scale;
	XMStoreFloat3(&this->scale, scaleVector);
	UpdateWorldMatrix();
}

//Change the scale with a float3
void Transform::ChangeScale(const XMFLOAT3& scale)
{
	this->scale.x += scale.x;
	this->scale.y += scale.y;
	this->scale.z += scale.z;
	scaleVector = XMLoadFloat3(&scale);
	UpdateWorldMatrix();
}

//Change the scale with 3 different floats
void Transform::ChangeScale(float x, float y, float z)
{
	scale.x += x;
	scale.y += y;
	scale.z += z;
	scaleVector = XMLoadFloat3(&scale);
	UpdateWorldMatrix();
}

//Set the look at for each object
void Transform::setLookAtPosition(XMFLOAT3 pos, bool noY)
{
	//If the pos past in == what it already is return out of the method
	if (pos.x == this->pos.x && pos.y == this->pos.y && pos.z == this->pos.z) {
		return;
	}

	//Set the pos past in to this pos, minus the pos past in to get the direction
	pos.x = this->pos.x - pos.x;
	pos.y = this->pos.y - pos.y;
	pos.z = this->pos.z - pos.z;

	//Set pitch to 0
	float pitch = 0.0f;

	//If we do not want to take into account the Y
	if (!noY) {
		//if pos.y doesnt = 0
		if (pos.y != 0) {
			//the distance becomes the square root of x*x + z*z
			float distance = sqrt((pos.x * pos.x) + (pos.z * pos.z));
			//the pitch is tan(y / distance)
			pitch = atan(pos.y / distance);
		}
	}
	//set yaw to 0
	float yaw = 0.0f;
	//if pos.x is not 0
	if (pos.x != 0.0f) {
		//the yaw is tan(x/z)
		yaw = atan(pos.x / pos.z);
	}
	//if pos z is greater than 0
	if (pos.z > 0) {
		//Yaw is yaw + pi
		yaw += XM_PI;
	}
	//Set the rotation of the object
	SetRotation(pitch, yaw, 0.0f);
}