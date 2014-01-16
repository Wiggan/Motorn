#include "ScopeEntity.h"
#include "Util.h"
#include "Camera.h"
ScopeEntity::ScopeEntity(const std::string &pName) : Entity(pName), mRayComponent(NULL), mTarget(NULL), mMaxPitch(0.05f), mMaxYaw(0.8f) {
    using namespace DirectX;
    mRotationPath.push_back(XMFLOAT3(mMaxPitch, mMaxYaw, 0.0f));
    mRotationPath.push_back(XMFLOAT3(-mMaxPitch, mMaxYaw, 0.0f));
    mRotationPath.push_back(XMFLOAT3(-mMaxPitch, -mMaxYaw, 0.0f));
    mRotationPath.push_back(XMFLOAT3(mMaxPitch, -mMaxYaw, 0.0f));
}
ScopeEntity::~ScopeEntity() {
}
void ScopeEntity::onRayCastHit(DirectX::XMFLOAT3 pPosition, DirectX::XMFLOAT3 pDirection, float pDistance, Entity* pTarget) {
    if ( pTarget != mParent && pTarget != this ) {
        mTarget = pTarget;
        if ( mRayComponent != NULL ) {
            mRayComponent->setScale(DirectX::XMFLOAT3(1.0f, 1.0f, pDistance+1));
        }
    }
}
void ScopeEntity::setRayComponent(Component* pComponent) {
    mRayComponent = pComponent;
    mRayComponent->setScale(DirectX::XMFLOAT3(1.0f, 1.0f, 200.0f));
}
void ScopeEntity::update(const float pDelta, const DirectX::XMFLOAT4X4 &pTransform, const bool pParentUpdated) {
    using namespace DirectX;
    XMFLOAT3 rotation;
    if ( mTarget != NULL ) {
        rotation = Util::dir2rot(Util::dir(getWorldPosition(), mTarget->getWorldPosition()));
        //if ( abs(rotation.x) > mMaxPitch || abs(rotation.y) > mMaxYaw ) {
        //    mTarget = NULL;
        //    mRayComponent->setScale(DirectX::XMFLOAT3(1.0f, 1.0f, 200.0f));
        //}
    } 
    if (mTarget == NULL) {
        rotation = Util::lerp(mRotationPath, 0.5f*(1 - cos(Timer::getInstance().getElapsed() / 700)), true);
    }
    //rotation = Util::dir2rot(Util::dir(getWorldPosition(), Camera::getInstance().getPosition()));
    //mRayComponent->setScale(DirectX::XMFLOAT3(1.0f, 1.0f, 200.0f));

    setRotation(rotation);
    Entity::update(pDelta, pTransform, pParentUpdated);
}