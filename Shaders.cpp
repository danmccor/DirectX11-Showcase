#include "Shaders.h"

bool VertexShader::Initialise(ID3D11Device* device, std::string shaderPath, D3D11_INPUT_ELEMENT_DESC* desc, UINT numElements)
{
	//Convert path to file into a wide string
	std::wstring temp = std::wstring(shaderPath.begin(), shaderPath.end());
	wShaderPath = temp.c_str();

	//Read the file to blob
	HRESULT hr = D3DReadFileToBlob(wShaderPath, &shaderBuffer);
	if (FAILED(hr)) return false;

	//Create the vertex shader
	hr = device->CreateVertexShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &shader);
	if (FAILED(hr)) return false;

	//Create the input layou
	 hr = device->CreateInputLayout(desc, numElements, shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), &inputLayout);
	if (FAILED(hr)) return false;

	return true;
}

bool PixelShader::Initialise(ID3D11Device* device, std::string shaderPath)
{
	//Conver the file path to a wide string
	std::wstring temp = std::wstring(shaderPath.begin(), shaderPath.end());
	wShaderPath = temp.c_str();

	//Read the file to blob
	HRESULT hr = D3DReadFileToBlob(wShaderPath, &shaderBuffer);
	if (FAILED(hr)) return false;

	//Create the pixel shader
	hr = device->CreatePixelShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &shader);
	if (FAILED(hr)) return false;

	return true;
}
