#pragma once
#include <iostream>
#include <string>
#include "../Vertex.h"
#include "Drawable.h"
#include "../Structs.h"
#include "Texture.h"
#include <vector>

class Sprite : public Drawable
{
private:
	std::vector<ID3D11ShaderResourceView*> mTextureViews;
public:
	Sprite(D3dStuff &stuff, Texture* pTexture);
	~Sprite();
	virtual void draw(const DirectX::XMFLOAT4X4 &transform);
};

