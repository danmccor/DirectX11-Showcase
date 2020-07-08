#pragma once
#include <DirectXMath.h>
#include "GameObject.h"
using namespace DirectX;

class Camera : public GameObject
{
	//Set public variables and functions
public: 
	Camera();
	//Set the camera details (fov, screen size, near clipping and far clipping
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);
	//Return the view matrix
	const XMMATRIX& GetViewMatrix() const { return viewMat; };
	//Return the projection matrix
	const XMMATRIX& GetProjectionMatrix() const { return projectionMat; };

private:
	//Update the world matrix of the camera
	void UpdateWorldMatrix();
	//Store the view matrix and projection matrix
	XMMATRIX viewMat;
	XMMATRIX projectionMat;
};

