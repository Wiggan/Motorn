#pragma once
#include "../Structs.h"
#include <string>

class Texture {
	ID3D11Resource* mTexture;
	ID3D11ShaderResourceView* mTextureView;
public:
	ID3D11ShaderResourceView* getTextureView();
	Texture(D3dStuff &stuff, const std::string &filename);
	~Texture();
};

