#include "Entity.h"
#include "../Camera.h"

Entity::Entity(const std::string &pName) : mName(pName) {
    using namespace DirectX;
    std::vector<WorldObject> mChildren;
    mUpdate = true;
    mParent = NULL;
    mBoundingSphere = BoundingSphere(mPosition, 2.0f);
}
Entity::~Entity() {
    mChildren.clear();
}
void Entity::addEntity(Entity* pChild) {
    mChildren.push_back(pChild);
    pChild->setParent(this);
}
void Entity::addComponent(Component* pChild) {
    mComponents.push_back(pChild);
    pChild->setOwner(this);
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
        mBoundingSphere.Center = mPosition;
    } 
    for ( auto it = mChildren.begin(); it != mChildren.end(); it++ ) {
        (*it)->update(pDelta, mTransform, updated);
    }
    for ( auto it = mComponents.begin(); it != mComponents.end(); it++ ) {
        (*it)->update(pDelta, mTransform, updated);
    }
}
void Entity::rayCast(const std::vector<Entity*> &pRayCastableEntities) {
    for ( auto it = mChildren.begin(); it != mChildren.end(); it++ ) {
        if ( this != *it ) {
            (*it)->rayCast(pRayCastableEntities);
        }
    }
    for ( auto it = mComponents.begin(); it != mComponents.end(); it++ ) {
        (*it)->rayCast(pRayCastableEntities);
    }
}
void Entity::draw() {
    for ( auto it = mChildren.begin(); it != mChildren.end(); it++ ) {
        (*it)->draw();
    }
    for ( auto it = mComponents.begin(); it != mComponents.end(); it++ ) {
        (*it)->draw();
    }
}
void Entity::setParent(Entity* pParent) {
    mParent = pParent;
}
DirectX::BoundingSphere& Entity::getBoundingSphere() {
    return mBoundingSphere;
}
void Entity::onRayCastHit(DirectX::XMFLOAT3 pPosition, DirectX::XMFLOAT3 pDirection, float pDistance, Entity* pTarget) {

}