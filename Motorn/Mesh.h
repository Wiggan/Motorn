#pragma once
#include <string>
#include "Drawable.h"
#include "Structs.h"

class Mesh : public Drawable
{
	ID3D11Buffer *iBuffer;
	int n;
public:
	Mesh(D3dStuff &stuff, const std::string &filename);
	~Mesh();
	virtual void draw(const DirectX::XMFLOAT4X4 &transform);
	void setPosition();
};

