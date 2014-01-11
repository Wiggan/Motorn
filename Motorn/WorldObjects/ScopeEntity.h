#pragma once
#include "Entity.h"
class ScopeEntity : public Entity {
    Component*                      mRayComponent;
    Entity*                         mTarget;
    float                           mMaxYaw;
    float                           mMaxPitch;
    std::vector<DirectX::XMFLOAT3>  mRotationPath;
public:
                                ScopeEntity(const std::string &pName);
                                ~ScopeEntity();
    void                        onRayCastHit(DirectX::XMFLOAT3 pPosition, DirectX::XMFLOAT3 pDirection, float pDistance, Entity* pTarget);
    void                        setRayComponent(Component* pComponent);
    virtual void                update(const float delta, const DirectX::XMFLOAT4X4 &pTransform, const bool parentUpdated);

};

