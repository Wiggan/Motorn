#include "WorldObject.h"
#include <iostream>
WorldObject::WorldObject(const std::string &pName) : mName(pName) {
    using namespace DirectX;
    mPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
    mScale = XMFLOAT3(1.0f, 1.0f, 1.0f);
    mRotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
    XMStoreFloat4x4(&mTransform, XMMatrixIdentity());
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
DirectX::XMFLOAT3 WorldObject::getDirection() {
    using namespace DirectX;
    XMFLOAT3 direction(mTransform._13, mTransform._23, mTransform._33);
    return direction;
}
DirectX::XMFLOAT3 WorldObject::getWorldPosition() {
    using namespace DirectX;
    XMFLOAT3 direction(mTransform._14, mTransform._24, mTransform._34);
    return direction;
}
void WorldObject::setPosition(const DirectX::XMFLOAT3 &pPosition) {
    mUpdate = true;
    mPosition = pPosition;
}
void WorldObject::setRotation(const DirectX::XMFLOAT3 &pRotation) {
    mUpdate = true;
    float pi2 = 3.14159265359f * 2;
    mRotation = pRotation;
    if ( mRotation.x > pi2 ) {
        mRotation.x -= pi2;
    } else if ( mRotation.x < 0 ) {
        mRotation.x += pi2;
    }
    if ( mRotation.y > pi2 ) {
        mRotation.y -= pi2;
    } else if ( mRotation.y < 0 ) {
        mRotation.y += pi2;
    }
    if ( mRotation.z > pi2 ) {
        mRotation.z -= pi2;
    } else if ( mRotation.z < 0 ) {
        mRotation.z += pi2;
    }
}
void WorldObject::setScale(const DirectX::XMFLOAT3 &pScale) {
    mUpdate = true;
    mScale = pScale;
}
const DirectX::XMFLOAT3& WorldObject::getPosition() {
    return mPosition;
}
const std::string& WorldObject::getName() {
    return mName;
}