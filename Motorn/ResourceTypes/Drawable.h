#pragma once
#include "../Vertex.h"
#include <iostream>

class Drawable {
protected:
	ID3D11Device *dev;
	ID3D11DeviceContext *devcon;
	ID3D11Buffer *pVBuffer;
	PerObjectBuffer constants;
	bool setShaderConstants(const PerObjectBuffer &constants) {
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(constants);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		ID3D11Buffer * buffer;
		HRESULT hr1 = dev->CreateBuffer(&bd, NULL, &buffer);       // create the buffer
		if ( FAILED(hr1) ) {
			std::cout << "Failed to create buffer! " << hr1 << std::endl;
			return false;
		}
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr2 = devcon->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if ( FAILED(hr2) ) {
			std::cout << "Failed mapping buffer to mapped resource!" << std::endl;
			return false;
		}
		memcpy(mappedResource.pData, &constants, sizeof(constants));
		devcon->Unmap(buffer, 0);
		devcon->PSSetConstantBuffers(0, 1, &buffer);
		devcon->VSSetConstantBuffers(0, 1, &buffer);
		return true;
	}
public:
	virtual ~Drawable() {}
	virtual void draw(const DirectX::XMFLOAT4X4 &transform) = 0;
};