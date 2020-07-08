#pragma once
#ifndef ConstantBuffer_h__
#define ConstantBuffer_h__
#include <d3d11.h>
#include <wrl/client.h>
#include "cBufferTypes.h"

//A template class to store the constant buffer
///Used as a template to ensure you can either pass in a vertex shader constant buffer or a pixel shader constant buffer
template<class T>
class ConstantBuffer
{
public:
	//Instantiate the buffer as null
	ConstantBuffer() {
		buffer = nullptr;
	}
	T data{};

	//Initialise the constant buffer
	HRESULT Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext) {
		//Set device context
		this->deviceContext = deviceContext;

		//Create a buffer desc
		CD3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		//Set desc values
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
		desc.StructureByteStride = 0;

		//Create the constant buffer
		HRESULT hr = device->CreateBuffer(&desc, 0, &buffer);
		return hr;
	}
	//Apply any changes to this->data to the constant buffer
	bool ApplyChanges() {
		//Create the subresource for the buffer
		D3D11_MAPPED_SUBRESOURCE mapRes;
		//Map the constant buffer to the subresource
		HRESULT hr = deviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);
		//if it fails return false
		if (FAILED(hr))return false;
		//Copy the memory of this data into the mapped resource
		CopyMemory(mapRes.pData, &data, sizeof(T));
		//Unmap the buffer
		deviceContext->Unmap(buffer, 0);
		return true;
	}

	//Return the ID3D11Buffer
	ID3D11Buffer* Get()const
	{
		return buffer;
	}
	//Return the address of the ID3D11Buffer
	ID3D11Buffer* const* GetAddress()const
	{
		return &buffer;
	}


private:
	//Initialise the variables
	ID3D11Buffer* buffer;
	ID3D11DeviceContext* deviceContext = nullptr;

};
#endif // !VertexBuffer_h__