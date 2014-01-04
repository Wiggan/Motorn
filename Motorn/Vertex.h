#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

struct ShaderConstants {
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;
	FLOAT color[4];
	FLOAT time;
	FLOAT black[3];
};



struct VERTEX
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 UV;
};

const D3D11_INPUT_ELEMENT_DESC ied[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//{ "TEXCOORD0", 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

