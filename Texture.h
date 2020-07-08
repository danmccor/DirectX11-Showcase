#pragma once
#include <d3d11.h>
#include "Colour.h"
#include <string>
#include <assimp/material.h>
#include <WICTextureLoader.h>


//Create enum for texture storage types
enum class TextureStorageType
{
	Invalid, 
	None, 
	EmbeddedIndexCompressed,
	EmbeddedIndexNonCompressed,
	EmbeddedCompressed, 
	EmbeddedNonCompressed,
	Disk
};


class Texture
{
public:
	//Overloaded constructor for different methods of texturing
	Texture(ID3D11Device* device, const Colour& colour, aiTextureType type);
	Texture(ID3D11Device* device, const Colour* colourData, UINT width, UINT height, aiTextureType type);
	Texture(ID3D11Device* device, const std::string & filePath, aiTextureType type);
	Texture(ID3D11Device* device, const uint8_t* data, size_t size, aiTextureType type);
	//Get type of texture for ASSIMP
	aiTextureType GetType() {return type;};
	//Get the shader resource view
	ID3D11ShaderResourceView* GetTextureResourceView() { return textureView; };
	//Get the shader resource view address
	ID3D11ShaderResourceView** GetTextureResourceViewAddress() { return &textureView; };

private:
	//Initalise the colour
	void Initialise1x1ColourTexture(ID3D11Device* device, const Colour& colourData, aiTextureType type);
	//Initialise material from ASSIMP
	void InitialiseColourTexture(ID3D11Device* device, const Colour* colourData, UINT width, UINT height, aiTextureType type);
	//Set texture variables
	ID3D11Resource* texture = nullptr;
	ID3D11ShaderResourceView* textureView = nullptr;
	aiTextureType type = aiTextureType::aiTextureType_UNKNOWN;

};

