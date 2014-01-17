#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "WorldObject.h"
#include "Component.h"
#include "Mesh.h"
#include <vector>
#include <DirectXCollision.h>
#include "Timer.h"
class Entity : public WorldObject {
protected:
    std::vector<Entity*>        mChildren;
    std::vector<Component*>     mComponents;
    std::vector<Component*>     mTransparentComponents;
    Entity*                     mParent;
    DirectX::BoundingSphere     mBoundingSphere;
public:
                                Entity(const std::string &pName);
                                Entity() : Entity("") {}
    virtual                     ~Entity();
    void                        addComponent(Component* pComponent);
    void                        addComponent(Mesh* pComponent);
    void                        addEntity(Entity* pEntity);
    virtual void                rayCast(const std::vector<Entity*> &pRayCastableEntities);
    virtual void                update(const float delta, const DirectX::XMFLOAT4X4 &pTransform, const bool parentUpdated);
    virtual void                draw();
    virtual void                drawTransparent();
    void                        setParent(Entity* pParent);
    DirectX::XMFLOAT4X4&        getTransform();
    DirectX::BoundingSphere&    getBoundingSphere();
    Component*                  getComponentByName(const std::string &pName);
    virtual void                onRayCastHit(DirectX::XMFLOAT3 pPosition, DirectX::XMFLOAT3 pDirection, float pDistance, Entity* pTarget);
};

