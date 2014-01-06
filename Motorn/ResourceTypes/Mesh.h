#pragma once
#include <string>
#include "Drawable.h"
#include "../Structs.h"
#include "Texture.h"
#include "MaterialResource.h"
#include <vector>
class Mesh : public Drawable
{
	ID3D11Buffer *iBuffer;
	std::vector<ID3D11ShaderResourceView*> mTextureViews;
	std::vector<Texture*> mTextures;
	MaterialResource** mMaterialResource;
	int n;
public:
	Mesh(D3dStuff &stuff, const std::string &filename, std::vector<Texture*> pTextures, MaterialResource** pMaterialResource);
	~Mesh();
	virtual void draw(const DirectX::XMFLOAT4X4 &transform);
	std::vector<Texture*> getTextures();
	MaterialResource** getMaterial();
	void setPosition();
};

