#include "AIEntity.h"
#include "Util.h"
AIEntity::AIEntity(const std::string &pName) : Entity(pName){
}
AIEntity::~AIEntity() {
}
void AIEntity::update(const float pDelta, const DirectX::XMFLOAT4X4 &pTransform, const bool pParentUpdated) {
    using namespace DirectX;
    XMFLOAT3 nextPosition = Util::bez(mWayPoints, 0.5f*(1 - cos(Timer::getInstance().getElapsed() / 1000)), false);
    XMFLOAT3 rotation = Util::dir2rot(Util::dir(getPosition(), nextPosition));
    setRotation(rotation);
    setPosition(nextPosition);
    Entity::update(pDelta, pTransform, pParentUpdated);
}
void AIEntity::addWayPoint(DirectX::XMFLOAT3 pPoint) {
    mWayPoints.push_back(pPoint);
}