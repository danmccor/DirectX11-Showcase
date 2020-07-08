#pragma once
#include "Transform.h"
#include <sstream>

class Terrain : public Transform
{
public:
	//Load the height map from a file
	bool LoadHeightMap(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string fileName, float heightScale, ConstantBuffer<vs_cBuffer>& vs_ConstantBuffer);
	//Initialise the terrain
	bool Initialise();
	//Get the relative height of the terrain
	float GetRelHeight(int x, int z);
	//Draw the terrain
	void Draw(const XMMATRIX& viewProjectionMatrix);

	//Store the width and length of the image
	int width;
	int length;
	float** heights;

	//Scale the heights by this variables
	float heightScale;

	//Store the height at each coordinate
	std::vector<std::vector<float>> newHeights;
	XMFLOAT3* heightMap;

	//Create a mesh for the terrain
	Mesh *mesh;

	//Store device and device context
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	//Create a constant buffer for terrain
	ConstantBuffer<vs_cBuffer>* vs_ConstantBuffer;

	//Store the vertices, indices and textures of the terrain
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;
	std::vector<Texture> textures;

	//Create a world matrix for the terrain
	XMMATRIX worldMat = XMMatrixIdentity();
};

