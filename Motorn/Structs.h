#pragma once
#include <D3D11.h>
#include <DirectXMath.h>
struct D3dStuff {
	ID3D11Device *dev;                     // the pointer to our Direct3D device interface
	ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
};
struct Material {
	Material() {
		ZeroMemory(this, sizeof(this));
	}
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular; // w = power, or shininess
};
struct DirectionalLight {
	DirectionalLight() {
		ZeroMemory(this, sizeof(this));
	}
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
	DirectX::XMFLOAT3 direction;
	float pad;
};

struct PerFrameBuffer {
	PerFrameBuffer() {
		ZeroMemory(this, sizeof(this));
	}
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;
	DirectionalLight directionalLight;
	DirectX::XMFLOAT3 cameraPosition;
	float pad1;
	double time;
	DirectX::XMFLOAT2 pad2;
};
struct PerObjectBuffer {
	PerObjectBuffer() {
		ZeroMemory(this, sizeof(this));
	}
	DirectX::XMFLOAT4X4 worldMatrix;
	Material material;
};
