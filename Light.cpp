#include "Light.h"

bool Light::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<vs_cBuffer>& cb_Vertex)
{
	//Initialise the lightbulb model
	model.Initialise("res/a.fbx", device, deviceContext, cb_Vertex);

	//Set the position, rotation and scale
	SetPosition(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f, 0.0f, 0.0f);
	SetScale(1.0f, 1.0f, 1.0f);
	//Update the world matrix
	UpdateWorldMatrix();
	return false;
}
