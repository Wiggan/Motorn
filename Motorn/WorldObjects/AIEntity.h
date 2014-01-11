#pragma once
#include "Entity.h"
#include <vector>
#include <directXMath.h>
class AIEntity : public Entity {
    std::vector<DirectX::XMFLOAT3> mWayPoints;
public:
                    AIEntity(const std::string &pName);
                    ~AIEntity();
    virtual void    update(const float delta, const DirectX::XMFLOAT4X4 &pTransform, const bool parentUpdated);

};

