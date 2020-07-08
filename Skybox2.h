#pragma once
#include <DDSTextureLoader.h>
#include <GeometricPrimitive.h>
#include <Effects.h>
#include "Camera.h"
#include "Shaders.h"
class Skybox2 : public Transform
{
public:
	Skybox2();
	//Initialise the skybox
	void Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	//Create the sphere for the skybox to draw onto
	void createSphere(int latLines, int longLines);
	//Draw the skybox
	void Draw(Camera camera);
	//Create the variables for the skybox
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;

	//Create the resource view and states
	ID3D11ShaderResourceView* srvSkyBox = nullptr;
	ID3D11Resource* skyBox = nullptr;
	ID3D11Texture2D* skyBoxTex = nullptr;
	ID3D11RasterizerState* rasterizerState = nullptr;
	ID3D11SamplerState* CubesTexSamplerState = nullptr;
	ID3D11DepthStencilView* dsView = nullptr;
	ID3D11DepthStencilState* dsState = nullptr;
	
	//initialise the number of vertices and faces of the skybox
	unsigned int NumSphereVertices;
	unsigned int NumSphereFaces;

	//Initialise the buffers
	ID3D11Buffer* skybox_vb;
	ID3D11Buffer* skybox_ib;


	//Initialise the constant buffer
	ConstantBuffer<vs_cBuffer> vs_constantBuffer;

	//Initialise the worldmat
	XMMATRIX sphere;
	//Initalise the rotatations for creating the sphere
	DirectX::XMMATRIX rotationX;
	DirectX::XMMATRIX rotationY;
	DirectX::XMMATRIX rotationZ;
};

