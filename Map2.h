#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "GameObject.h"
#include "Camera.h"
#include "Light.h"

struct InstanceData
{
	XMFLOAT3 pos;
};

class Map2
{
public:
	//Initialise the map
	bool Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<vs_cBuffer>& vs_constantBuffer);
	//Load the map from a file
	bool LoadMap(std::string fileName);
	//Draw the map
	void Draw(Camera camera);
	//Vector of all the buildings in the map
	std::vector<GameObject> gameObjects;
	std::vector<Light*> lights;


private:
	//Variables for map creation
	std::string name;
	int width;
	int length;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ConstantBuffer<vs_cBuffer>* vs_constantBuffer;
};

