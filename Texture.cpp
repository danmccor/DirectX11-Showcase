#include "Texture.h"

//Constructor for Texture 
Texture::Texture(ID3D11Device* device, const Colour& colour, aiTextureType type) {

	//Initialise a 1x1 colour texture
	Initialise1x1ColourTexture(device, colour, type);
}
//Constructor for texture
Texture::Texture(ID3D11Device* device, const Colour* colourData, UINT width, UINT height, aiTextureType type) {
	//Initialise the colour texture
	InitialiseColourTexture(device, colourData, width, height, type);
}

Texture::Texture(ID3D11Device* device, const std::string& filePath, aiTextureType type)
{
	//Set the type of this texture
	this->type = type;
	//Convert the string to a wide string
	std::wstring wfileName = std::wstring(filePath.begin(), filePath.end());
	//Create the texture from a file
	HRESULT hr = DirectX::CreateWICTextureFromFile(device, wfileName.c_str(), &texture, &textureView);
	if (FAILED(hr)) {
		//If it fails set the colour to unhandled texture colour
		Initialise1x1ColourTexture(device, Colours::UnloadedTextureColour, type);
	}
}

Texture::Texture(ID3D11Device* device, const uint8_t* data, size_t size, aiTextureType type)
{
	//Set type
	this->type = type;
	//Load the texture from memory
	DirectX::CreateWICTextureFromMemory(device, data, size, &texture, &textureView, type);
}

void Texture::Initialise1x1ColourTexture(ID3D11Device* device, const Colour& colourData, aiTextureType type)
{
	//Set width and height to 1 and initialise a colour
	InitialiseColourTexture(device, &colourData, 1, 1, type);
}
void Texture::InitialiseColourTexture(ID3D11Device* device, const Colour* colourData, UINT width, UINT height, aiTextureType type)
{
	//Set type 
	this->type = type;
	//Create a texture 2D desc
	CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
	//Create a texture 2D
	ID3D11Texture2D* p2DTexture = nullptr;
	//create subresource data
	D3D11_SUBRESOURCE_DATA initialData{};
	//Set the data
	initialData.pSysMem = colourData;
	initialData.SysMemPitch = width * sizeof(Colour);
	//Create a 2d texture
	HRESULT hr = device->CreateTexture2D(&textureDesc, &initialData, &p2DTexture);
	//Cast as 2D texture
	texture = static_cast<ID3D11Texture2D*>(p2DTexture);
	//Create resource view desc
	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
	//Create the resource view
	device->CreateShaderResourceView(texture, &srvDesc, &textureView);
}

