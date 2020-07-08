#include "Map.h"

Map::Map()
{
}

bool Map::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<vs_cBuffer>& vs_constantBuffer) {
	this->device = device;
	this->deviceContext = deviceContext;
	this->vs_constantBuffer = &vs_constantBuffer;
	return true;
}

void Map::LoadMap(std::string fileName)
{
	std::string fileLine;
	std::ifstream file(fileName);
	if (file.is_open()) {
		while (!file.eof()) {
			std::getline(file, fileLine);
			std::istringstream ss(fileLine);
			ss >> name >> position.x >> position.y >> position.z >> scale.x >> scale.y >> scale.z >> rotation.x >> rotation.y >> rotation.z;
			GameObject gameObject;
			gameObject.Initialise(name, device, deviceContext, *vs_constantBuffer);
			gameObject.SetTransform(position, scale, rotation);
			gameObjects.push_back(gameObject);
		}
	}
	file.close();
}

void Map::Draw(Camera camera)
{
	for (int i = 0; i < gameObjects.size(); i++){
		gameObjects[i].Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());
	}
}
