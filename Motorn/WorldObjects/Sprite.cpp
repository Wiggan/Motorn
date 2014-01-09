#include "Sprite.h"
#include "../Camera.h"



Sprite::Sprite(D3dStuff &stuff, Texture* pTexture)
{
	using namespace DirectX;
	dev = stuff.dev;
	devcon = stuff.devcon;

	mTextureViews.push_back(pTexture->getTextureView());

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


void Sprite::draw()
{
	using namespace DirectX;
	constants.worldMatrix = mTransform;
	setShaderConstants(constants);
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	devcon->PSSetShaderResources(0, 1, &mTextureViews[0]);
	devcon->Draw(4, 0); 
}

const DirectX::XMFLOAT4X4& Sprite::getTransform() {
	using namespace DirectX;
	XMFLOAT3 up(0.0f, 1.0f, 0.0f);
	XMStoreFloat4x4(&mTransform,
		XMMatrixTranspose(
		XMMatrixScalingFromVector(XMLoadFloat3(&mScale))*
		XMMatrixLookAtLH(XMLoadFloat3(&mPosition),
		XMLoadFloat3(&Camera::getInstance().getPosition()),
		XMLoadFloat3(&up))*
		XMMatrixTranslationFromVector(XMLoadFloat3(&mPosition))));
	
	
	return mTransform;
}
