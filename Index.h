#ifndef IndicesBuffer_h__
#define IndicesBuffer_h__
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

//Class for index buffer
class IndexBuffer
{
private:
	IndexBuffer(const IndexBuffer& rhs);

private:
	//Variables for buffer and size
	ID3D11Buffer* buffer;
	UINT bufferSize = 0;

public:
	//Initialise buffer as null
	IndexBuffer() {
		buffer = nullptr;
	}
	//Initialise the buffer
	HRESULT Initialise(ID3D11Device* device, DWORD* data, UINT numIndices) {
		//Buffer size = the number of indicies in the buffer
		bufferSize = numIndices;

		//Create buffer desc
		CD3D11_BUFFER_DESC iBufferDesc;
		ZeroMemory(&iBufferDesc, sizeof(iBufferDesc));
		//Set buffer desc values
		iBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		iBufferDesc.ByteWidth = sizeof(DWORD) * numIndices;
		iBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		iBufferDesc.CPUAccessFlags = 0;
		iBufferDesc.MiscFlags = 0;

		//Create subresource for the buffer
		D3D11_SUBRESOURCE_DATA iBufferData;
		ZeroMemory(&iBufferData, sizeof(iBufferData));
		//Buffer data = this->data
		iBufferData.pSysMem = data;

		//Create the buffer
		HRESULT hr = device->CreateBuffer(&iBufferDesc, &iBufferData, &this->buffer);
		return hr;
	}

	//Return the index buffer
	ID3D11Buffer* Get()const
	{
		return buffer;
	}
	//Return the index buffer address
	ID3D11Buffer* const* GetAddress()const
	{
		return &buffer;
	}
	//Return the buffer size
	UINT BufferSize() const {
		return bufferSize;
	}

};
#endif