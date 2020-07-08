#pragma once
#include "GameObject.h"
class Light : public GameObject
{
public:
	//Initialise the light
	bool Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<vs_cBuffer>& cb_Vertex);
	//Set the light variables 
	DirectX::XMFLOAT3 lightColour = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	float lightStrength = 1.0f;
	//Attenuation is the light falloff, I speak about what this is in the report
	float attenuation_a = 0.4f;
	float attenuation_b = 0.02f;
	float attenuation_c = 0.0f;
};

