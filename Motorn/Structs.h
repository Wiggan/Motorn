#pragma once
#include <D3D11.h>

struct D3dStuff {
	ID3D11Device *dev;                     // the pointer to our Direct3D device interface
	ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
};
