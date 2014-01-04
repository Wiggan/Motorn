#include "Sprite.h"
#include "Camera.h"
#include <vector>


Sprite::Sprite(D3dSpriteStuff &stuff)
{
	using namespace DirectX;
	dev = stuff.dev;
	devcon = stuff.devcon;
	pPS = stuff.pPS;
	pVS = stuff.pVS;

	VERTEX OurVertices[] =
	{
		{ XMFLOAT3(-0.5f, 0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.5f, 0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) }
	};

	std::vector<uint16_t> indexes = {
		0, 1, 2,
		1, 2, 3,
	};

	constants.time = 0;
	XMStoreFloat4x4(&constants.worldMatrix, XMMatrixIdentity());
	constants.projectionMatrix = Camera::getInstance().getProjection();
	constants.viewMatrix = Camera::getInstance().getView();
	constants.color[0] = 1.0f * sin(constants.time / 2000.0f);
	constants.color[1] = 1.0f * sin(constants.time / 2000.0f + 120);
	constants.color[2] = 1.0f * sin(constants.time / 2000.0f + 34);
	constants.color[3] = 1.0f;
	setShaderConstants(constants);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(VERTEX) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	
	dev->CreateBuffer(&bd, NULL, &pVBuffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));
	devcon->Unmap(pVBuffer, NULL);
}


Sprite::~Sprite()
{
}


void Sprite::draw(const DirectX::XMFLOAT4X4 &transform)
{
	using namespace DirectX;
	//std::cout << "delta: " << delta << std::endl;
	constants.worldMatrix = transform;
	constants.projectionMatrix = Camera::getInstance().getProjection();
	constants.viewMatrix = Camera::getInstance().getView();
	constants.time += 16.6f;
	constants.color[0] = 1.0f * sin(constants.time / 2000.0f);
	constants.color[1] = 1.0f * sin(constants.time / 2000.0f + 120);
	constants.color[2] = 1.0f * sin(constants.time / 2000.0f + 34);
	constants.color[3] = 1.0f;
	setShaderConstants(constants);

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
	devcon->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R16_UINT, 0);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	devcon->Draw(4, 0); 
}

