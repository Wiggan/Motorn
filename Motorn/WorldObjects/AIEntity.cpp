#include "AIEntity.h"
#include "Util.h"

AIEntity::AIEntity(const std::string &pName) : Entity(pName){
    using namespace DirectX;
    mWayPoints.push_back(XMFLOAT3(0.0f, 0.0f, 5.0f));
    mWayPoints.push_back(XMFLOAT3(4.0f, 0.0f, 5.0f));
    mWayPoints.push_back(XMFLOAT3(5.0f, 0.0f, 0.0f));
    mWayPoints.push_back(XMFLOAT3(4.0f, 1.0f, -5.0f));
    mWayPoints.push_back(XMFLOAT3(0.0f, 2.0f, -5.0f));
    mWayPoints.push_back(XMFLOAT3(-4.0f, 3.0f, 0.0f));
    mWayPoints.push_back(XMFLOAT3(-5.0f, 4.0f, 5.0f));
    mWayPoints.push_back(XMFLOAT3(0.0f, 5.0f, 5.0f));
    mWayPoints.push_back(XMFLOAT3(0.0f, 9.0f, 5.0f));

}


AIEntity::~AIEntity() {
}
void AIEntity::update(const float pDelta, const DirectX::XMFLOAT4X4 &pTransform, const bool pParentUpdated) {
    using namespace DirectX;
    XMFLOAT3 nextPosition = Util::bez(mWayPoints, 0.5f*(1 - cos(Timer::getInstance().getElapsed() / 1000)), false);
    XMFLOAT3 direction;
    XMStoreFloat3(&direction, XMVector3Normalize(XMLoadFloat3(&getPosition()) - XMLoadFloat3(&nextPosition)));
    //XMStoreFloat3(&rotation, XMVector3AngleBetweenNormals(XMLoadFloat3(&direction), XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 1.0f))));
    XMFLOAT3 rotation = Util::dir2rot(direction);
    setRotation(rotation);
    setPosition(nextPosition);
    Entity::update(pDelta, pTransform, pParentUpdated);
}