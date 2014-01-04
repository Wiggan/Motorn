#pragma once
#include <D3D11.h>

struct D3dSpriteStuff {
	ID3D11Device *dev;                     // the pointer to our Direct3D device interface
	ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
	ID3D11VertexShader *pVS;    // the vertex shader
	ID3D11PixelShader *pPS;     // the pixel shader
};
