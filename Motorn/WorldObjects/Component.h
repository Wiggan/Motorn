#pragma once
#include "WorldObject.h"
#include <vector>
class Entity;
class Component : public WorldObject {
protected:
    Entity*            mOwner;
public:
    virtual void    update(const float pDelta, const DirectX::XMFLOAT4X4 &pTransform, const bool pParentUpdated);
    void            Component::setOwner(Entity* pOwner);
    virtual void    rayCast(const std::vector<Entity*> &pRayCastableEntities) {}
};