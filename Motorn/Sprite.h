#pragma once
#include <iostream>
#include <string>
#include "Vertex.h"
#include "Drawable.h"
#include "Structs.h"

class Sprite : public Drawable
{
private:
	ShaderConstants constants;

public:
	Sprite(D3dStuff &stuff);
	~Sprite();
	virtual void draw(const DirectX::XMFLOAT4X4 &transform);
};

