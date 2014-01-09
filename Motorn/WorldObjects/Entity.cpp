#include "Entity.h"
#include "../Camera.h"

Entity::Entity(const std::string &pName) : mName(pName) {
	using namespace DirectX;
	std::vector<WorldObject> mChildren;
	mUpdate = true;
	mParent = NULL;
}
Entity::~Entity() {
	mChildren.clear();
}
void Entity::addEntity(Entity* pChild) {
	mChildren.push_back(pChild);
	pChild->setParent(this);
}
void Entity::addComponent(Component* pChild) {
	mChildren.push_back(pChild);
	pChild->setOwner(this);
}
void Entity::draw() {
	for ( auto it = mChildren.begin(); it != mChildren.end(); it++ ) {
		(*it)->draw();
	}
}
void Entity::update(const float pDelta, const DirectX::XMFLOAT4X4 &pTransform, const bool pParentUpdated) {
	using namespace DirectX;
	bool updated = false;
	if ( mUpdate || pParentUpdated ) {
		XMStoreFloat4x4(&mTransform,
			XMMatrixTranspose(
			XMMatrixScalingFromVector(XMLoadFloat3(&mScale))*
			XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&mRotation))*
			XMMatrixTranslationFromVector(XMLoadFloat3(&mPosition))));
		mUpdate = false;
		XMStoreFloat4x4(&mTransform, XMLoadFloat4x4(&pTransform)*XMLoadFloat4x4(&mTransform));
		updated = true;
	} 
	for ( auto it = mChildren.begin(); it != mChildren.end(); it++ ) {
		(*it)->update(pDelta, mTransform, updated);
	}
}
void Entity::setPosition(const DirectX::XMFLOAT3 &pPosition) {
	mUpdate = true;
	mPosition = pPosition;
}
void Entity::setRotation(const DirectX::XMFLOAT3 &pRotation) {
	mUpdate = true;
	mRotation = pRotation;
}
void Entity::setScale(const DirectX::XMFLOAT3 &pScale) {
	mUpdate = true;
	mScale = pScale;
}
void Entity::setParent(Entity* pParent) {
	mParent = pParent;
}

