#ifndef VertexBuffer_h__
#define VertexBuffer_h__
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

//Create the struct for the vertex. This is the pattern a vertex must follow
struct Vertex
{
	Vertex() : pos(0, 0, 0), texCoord(0, 0), normal(0, 0, 0) {
		pos = DirectX::XMFLOAT3(0, 0, 0);
		texCoord = DirectX::XMFLOAT2(0, 0);
		normal = DirectX::XMFLOAT3(0, 0, 0);
	};
	Vertex(float x, float y, float z, float u, float v,float nx, float ny, float nz) : pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}
	DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(0,0,0);
	DirectX::XMFLOAT2 texCoord = DirectX::XMFLOAT2(0, 0);
	DirectX::XMFLOAT3 normal = DirectX::XMFLOAT3(0, 0, 0);
};



//Create a template class for the vertex, pushing in a customer buffer
template<class T>
class VertexBuffer 
{
private:
	//Set the stride, buffer and buffer size
	ID3D11Buffer* buffer;
	UINT stride = sizeof(T); 
	UINT bufferSize = 0;

public:
	VertexBuffer() {}

	HRESULT Initialise(ID3D11Device* device, T* data, UINT numVertices) {
		bufferSize = numVertices;

		CD3D11_BUFFER_DESC vBufferDesc;
		ZeroMemory(&vBufferDesc, sizeof(vBufferDesc));

		vBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vBufferDesc.ByteWidth = stride * numVertices;
		vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vBufferDesc.CPUAccessFlags = 0;
		vBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vBufferData;
		ZeroMemory(&vBufferData, sizeof(vBufferData));
		vBufferData.pSysMem = data;

		HRESULT hr = device->CreateBuffer(&vBufferDesc, &vBufferData, &this->buffer);
		return hr;
	}

	ID3D11Buffer* Get()const
	{
		return buffer;
	}
	ID3D11Buffer* const* GetAddress()const
	{
		return &buffer;
	}
	UINT BufferSize() const {
		return bufferSize;
	}
	const UINT Stride() const
	{
		return stride;
	}
	//To set buffers with pointers
	const UINT* StridePtr() const
	{
		return &stride;
	}
	

};
#endif // !VertexBuffer_h__