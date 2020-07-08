#pragma once
#include <DirectXMath.h>
#include "Mesh.h"
#include <vector>
#include <iostream>

using namespace DirectX;

class Model
{
public:
	//Initialise the model class
	bool Initialise(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<vs_cBuffer>& vs_ConstantBuffer);
	//Draw the model
	void Draw(const XMMATRIX& worldMat, const XMMATRIX& viewProjectionMatrix);
	//Get the models bounding box
	std::vector<XMFLOAT3> GetBoundingBox() { return boundingBox; };
	//Get the minimum and maxiumum position of the bounding boxes
	XMFLOAT3 GetMinimumPos() { return minimumPos; };
	XMFLOAT3 GetMaximumPos() { return maximumPos; };

private:
	//Create a bounding box for the model
	void CreateBoundingBox();

	//Set the minumum and maximum vertex position for the model. 
	//FLT_MAX is used to initialise the variable but ensure that the vertex can still be lower/greater than what it is initialised as:
	///Eg. if x < minimumpos(FLT_MAX) {minimumpos = x} will run
	XMFLOAT3 minimumPos = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	XMFLOAT3 maximumPos = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	//Create variable for its bounding box
	std::vector<XMFLOAT3> boundingBox;

	//Create variables for model
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	ConstantBuffer<vs_cBuffer>* vs_ConstantBuffer = nullptr;
	
	//Create vector to store the models meshes
	std::vector<Mesh> meshes;

	//Load the model
	bool LoadModel(const std::string& filePath);
	//Process the models nodes
	void ProcessNode(aiNode* node, const aiScene* scene, const XMMATRIX& parentTransform);
	//Process the models meshes
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const XMMATRIX& transformMatrix);
	//Find out what storage type the model is using (disk, compressed index etc)
	TextureStorageType DetermineTextureStorageType(const aiScene* scene, aiMaterial* material, unsigned int index, aiTextureType type);
	//Load the models textures
	std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, const aiScene* scene);
	//Get the texture location
	int GetTextureIndex(aiString* string);
	//Set the file path for the models
	std::string filePath ="";
	int numOfIndices = 0;

};