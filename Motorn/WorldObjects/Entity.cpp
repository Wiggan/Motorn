#include "Entity.h"
#include "../Camera.h"
#include <iostream>
#include <algorithm>
Entity::Entity(const std::string &pName) : WorldObject(pName) {
    using namespace DirectX;
    std::vector<WorldObject> mChildren;
    mUpdate = true;
    mParent = NULL;
    mBoundingSphere = BoundingSphere(mPosition, 2.0f);
}
Entity::~Entity() {
    std::cout << "Removing Entity" << std::endl;
    for ( auto it = mComponents.begin(); it != mComponents.end(); it++ ) {
        delete (*it);
    }
    mComponents.clear();
    for ( auto it = mChildren.begin(); it != mChildren.end(); it++ ) {
        delete (*it);
    }
    mChildren.clear();
}
void Entity::addEntity(Entity* pChild) {
    mChildren.push_back(pChild);
    pChild->setParent(this);
}
void Entity::addComponent(Component* pChild) {
    pChild->setOwner(this);
    mComponents.push_back(pChild);
}
void Entity::addComponent(Mesh* pChild) {
    pChild->setOwner(this);
    if ( pChild->getMaterial()->diffuse.w < 1.0f ) {
        mTransparentComponents.push_back(pChild);
    } else {
        mComponents.push_back(pChild);
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
        mBoundingSphere.Center = mPosition;
    } 
    for ( auto it = mChildren.begin(); it != mChildren.end(); it++ ) {
        (*it)->update(pDelta, mTransform, updated);
    }
    for ( auto it = mComponents.begin(); it != mComponents.end(); it++ ) {
        (*it)->update(pDelta, mTransform, updated);
    }
    for ( auto it = mTransparentComponents.begin(); it != mTransparentComponents.end(); it++ ) {
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
void Entity::drawTransparent() {
    for ( auto it = mChildren.begin(); it != mChildren.end(); it++ ) {
        (*it)->drawTransparent();
    }
    for ( auto it = mTransparentComponents.begin(); it != mTransparentComponents.end(); it++ ) {
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
Component* Entity::getComponentByName(const std::string &pName) {
    for ( auto it = mComponents.begin(); it != mComponents.end(); it++ ) {
        if ( strcmp((*it)->getName().c_str(), pName.c_str()) == 0 ) {
            return (*it);
        }
    }
    for ( auto it = mTransparentComponents.begin(); it != mTransparentComponents.end(); it++ ) {
        if ( strcmp((*it)->getName().c_str(), pName.c_str()) == 0 ) {
            return (*it);
        }
    }
    return NULL;
}