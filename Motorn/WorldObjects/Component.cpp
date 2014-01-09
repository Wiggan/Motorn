#include "Component.h"

#include "Entity.h"

void Component::update(const float pDelta, const DirectX::XMFLOAT4X4 &pTransform, const bool pParentUpdated) {
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
void Component::setOwner(Entity* pOwner) {
	mOwner = pOwner;
}
