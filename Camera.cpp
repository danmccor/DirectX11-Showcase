#include "Camera.h"

Camera::Camera()
{
	//Set the camera's position
	pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	posVector = XMLoadFloat3(&pos);
	//Set the camera's rotation
	rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotVector = XMLoadFloat3(&rot);
	//Update the Camera's world Matrix
	UpdateWorldMatrix();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	//Convert the cameras field of view from degrees to radians
	float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
	//Set the cameras projection matrix
	projectionMat = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

void Camera::UpdateWorldMatrix()
{
	//set the camera's rotation matrix
	XMMATRIX camRotationMat = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	//Rotate the camera's forward vector
	XMVECTOR camTarget = XMVector3TransformCoord(forwardVec, camRotationMat);
	//Add the cameras position
	camTarget += posVector;
	//Rotate the camera's up vector
	XMVECTOR up = XMVector3TransformCoord(upVec, camRotationMat);
	//Set the camra's view matrix by its rotated position
	viewMat = XMMatrixLookAtLH(posVector, camTarget, up);

	//Update the direction Vectors
	UpdateDirectionVectors();
}
