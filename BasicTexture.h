#pragma once
#include "Model.h"
#include <WICTextureLoader.h>

class BasicTexture
{
public:
	bool Load(std::string FileName, ID3D11Device* GraphicsDevice);
	ID3D11ShaderResourceView* ResourceView();
	ID3D11Texture2D* Texture2D();

private:
	ID3D11ShaderResourceView* ShaderResourceView;
	ID3D11Texture2D* texture;
};

