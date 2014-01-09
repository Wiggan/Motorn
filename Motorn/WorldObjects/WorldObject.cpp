#include "WorldObject.h"

WorldObject::WorldObject() {
	using namespace DirectX;
	mPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mScale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	mRotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void WorldObject::update(const float pDelta, const DirectX::XMFLOAT4X4 &pTransform, const bool pParentUpdated) {
	using namespace DirectX;
	if ( mUpdate || pParentUpdated ) {
		XMStoreFloat4x4(&mTransform,
			XMMatrixTranspose(
			XMMatrixScalingFromVector(XMLoadFloat3(&mScale))*
			XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&mRotation))*
			XMMatrixTranslationFromVector(XMLoadFloat3(&mPosition))));
		mUpdate = false;
		XMStoreFloat4x4(&mTransform, XMLoadFloat4x4(&pTransform)*XMLoadFloat4x4(&mTransform));
	}
}

void WorldObject::setPosition(const DirectX::XMFLOAT3 &pPosition) {
	mUpdate = true;
	mPosition = pPosition;
}
void WorldObject::setRotation(const DirectX::XMFLOAT3 &pRotation) {
	mUpdate = true;
	mRotation = pRotation;
}
void WorldObject::setScale(const DirectX::XMFLOAT3 &pScale) {
	mUpdate = true;
	mScale = pScale;
}
