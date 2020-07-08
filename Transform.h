#pragma once
#include "Model.h"

//The transform class GameObject inherits from
class Transform
{
public:
	Transform();
	//Get the position, rotation and scale vectors or float3s
	virtual const XMVECTOR& GetPositionVector() const { return posVector; };
	virtual const XMFLOAT3& GetPositionFloat3() const { return pos; };
	virtual const XMVECTOR& GetRotationVector() const { return rotVector; };
	virtual const XMFLOAT3& GetRotationFloat3() const { return rot; };
	virtual const XMVECTOR& GetScaleVector() const { return scaleVector; };
	virtual const XMFLOAT3& GetScaleFloat3() const { return scale; };

	//Get the forward, right, back and left vectors
	//These change as the player rotates, forward for the player could be x:12 Z: 88.
	virtual const XMVECTOR& GetForwardVec(bool omitY = false) { if (omitY) { return forward_noY; } else { return forward; }; };
	virtual const XMVECTOR& GetRightVec(bool omitY = false) { if (omitY) { return right_noY; } else { return right; }; };
	virtual const XMVECTOR& GetBackwardVec(bool omitY = false) { if (omitY) { return backward_noY; } else { return backward; }; }
	virtual const XMVECTOR& GetLeftVec(bool omitY = false) { if (omitY) { return left_noY; } else { return left; }; }
	//Get the world matrix
	virtual const XMMATRIX& GetWorldMatrix() { return worldMat; };

	//Set the transforms position, rotation and scales
	virtual void SetPosition(const XMVECTOR& pos);
	virtual void SetPosition(const XMFLOAT3& pos);
	virtual void SetPosition(float x, float y, float z);
	virtual void ChangePosition(const XMVECTOR& pos);
	virtual void ChangePosition(const XMFLOAT3& pos);
	virtual void ChangePosition(float x, float y, float z);

	virtual void SetRotation(const XMVECTOR& rot);
	virtual void SetRotation(const XMFLOAT3& rot);
	virtual void SetRotation(float x, float y, float z);
	virtual void ChangeRotation(const XMVECTOR& rot);
	virtual void ChangeRotation(const XMFLOAT3& rot);
	virtual void ChangeRotation(float x, float y, float z);

	virtual void SetScale(const XMVECTOR& pos);
	virtual void SetScale(const XMFLOAT3& pos);
	virtual void SetScale(float x, float y, float z);
	virtual void ChangeScale(const XMVECTOR& pos);
	virtual void ChangeScale(const XMFLOAT3& pos);
	virtual void ChangeScale(float x, float y, float z);

	//Set the look at position, this will cause the objects to rotate towards position. Y is ommited by default so the object does not rotate up and down
	virtual void setLookAtPosition(XMFLOAT3 pos, bool noY = true);
protected:
	//Update the world matrix
	virtual void UpdateWorldMatrix();
	//Update the direction vectors
	void UpdateDirectionVectors();
	//Set the world mate
	XMMATRIX worldMat = XMMatrixIdentity();

	//Initialise all variables
	XMVECTOR posVector;
	XMVECTOR rotVector;
	XMVECTOR scaleVector;
	XMFLOAT3 pos;
	XMFLOAT3 rot;
	XMFLOAT3 scale;

	const XMVECTOR forwardVec = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR upVec = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR backVec = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR rightVec = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR leftVec = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR left= XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR backward = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);

	XMVECTOR forward_noY = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR left_noY= XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR right_noY = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR backward_noY = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);

};

