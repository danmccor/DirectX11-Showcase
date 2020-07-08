#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "GameObject.h"
#include "Camera.h"
class Map
{
public:
	Map();
	//Initialise the Map
	bool Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<vs_cBuffer>& vs_constantBuffer);
	//Load the Map
	void LoadMap(std::string fileName);
	//Draw the map
	void Draw(Camera camera);
private:
	//Set vector of game objects
	std::vector<GameObject> gameObjects;
	std::string name;
	//GameObject* Object;
	//Set position variables
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ConstantBuffer<vs_cBuffer>* vs_constantBuffer;
};

