#pragma once
#include <d3d11.h>
#include "../Structs.h"
#include <iostream>

class Shader {
private:
	ID3D11VertexShader *mVertexShader;
	ID3D11PixelShader *mPixelShader;
	ID3D11Device *dev;
	ID3D11DeviceContext *devcon;
	char* shader;
	unsigned int size;
public:
	void load();
	Shader(D3dStuff &stuff, const std::string &filename);
	~Shader();
};

