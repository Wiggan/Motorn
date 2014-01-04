#include "Entity.h"


Entity::Entity(const std::string &pName, Drawable* pDrawable) : mName(pName), mDrawable(pDrawable) {
	using namespace DirectX;
	std::vector<Entity*> children();
	mUpdate = true;
	mParentUpdate = false;
	mPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mScale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	mRotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMStoreFloat4x4(&mParentTransform, XMMatrixIdentity());
	mParent = NULL;
}
Entity::~Entity() {
	mChildren.clear();
}
void Entity::add(Entity &pChild) {
	mChildren.push_back(&pChild);
	pChild.setParent(*this);
}
void Entity::setParent(Entity &pParent) {
	mParent = &pParent;
}
void Entity::update(const double delta) {
	for ( auto it = mChildren.begin(); it != mChildren.end(); it++ ) {
		(*it)->update(delta);
	}
}
void Entity::draw() {
	using namespace DirectX;
	//auto transform = getTransform();
	if ( mDrawable != NULL )  {
		XMFLOAT4X4 transform;
		XMStoreFloat4x4(&transform, XMLoadFloat4x4(&mParentTransform)*XMLoadFloat4x4(&getTransform()));
		mDrawable->draw(transform);
	} else {
		getTransform(); // Update anyway, gotta do that shit, but probably elsewhere
	}
	for ( auto it = mChildren.begin(); it != mChildren.end(); it++ ) {
		(*it)->draw();
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
void Entity::setParentTransform(const DirectX::XMFLOAT4X4 &pParentTransform) {
	//mParentUpdate = true;
	mParentTransform = pParentTransform;
}
DirectX::XMFLOAT4X4& Entity::getTransform() {
	using namespace DirectX;
	if ( mUpdate ) {
		XMStoreFloat4x4(&mTransform, 
			XMMatrixTranspose(
			XMMatrixScalingFromVector(XMLoadFloat3(&mScale))*
			XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&mRotation))*
			XMMatrixTranslationFromVector(XMLoadFloat3(&mPosition))));
		mUpdate = false;
	}
	//if ( mParentUpdate ) {
		for ( auto it = mChildren.begin(); it != mChildren.end(); it++ ) {
			XMFLOAT4X4 totalParentTransform = mTransform;
			if ( mParent != NULL ) {
				XMStoreFloat4x4(&totalParentTransform, XMLoadFloat4x4(&mParentTransform)*XMLoadFloat4x4(&mTransform));
			}
			(*it)->setParentTransform(totalParentTransform);
		}
	//	mParentUpdate = false;
	//}
	return mTransform;
}




