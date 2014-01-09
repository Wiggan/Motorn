#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
class WorldObject {
protected:
	DirectX::XMFLOAT3	mPosition;
	DirectX::XMFLOAT3	mRotation;
	DirectX::XMFLOAT3	mScale;
	DirectX::XMFLOAT4X4 mTransform;
	bool				mUpdate;
						WorldObject();
public:
	virtual void		setPosition(const DirectX::XMFLOAT3 &pPosition);
	virtual void		setRotation(const DirectX::XMFLOAT3 &pRotation);
	virtual void		setScale(const DirectX::XMFLOAT3 &pScale);
	virtual void		update(const float delta, const DirectX::XMFLOAT4X4 &pTransform, const bool parentUpdated);
	virtual void		draw() = 0;//{}
	virtual				~WorldObject() {}
};