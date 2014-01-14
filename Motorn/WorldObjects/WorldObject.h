#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
class WorldObject {
protected:
    DirectX::XMFLOAT3       mPosition;
    DirectX::XMFLOAT3       mRotation;
    DirectX::XMFLOAT3       mScale;
    DirectX::XMFLOAT4X4     mTransform;
    std::string             mName;
    bool                    mUpdate;
                            WorldObject(const std::string &pName);
public:
    virtual void                setPosition(const DirectX::XMFLOAT3 &pPosition);
    virtual void                setRotation(const DirectX::XMFLOAT3 &pRotation);
    virtual void                setScale(const DirectX::XMFLOAT3 &pScale);
    const DirectX::XMFLOAT3&    getPosition();
    DirectX::XMFLOAT3           getDirection();
    DirectX::XMFLOAT3           getWorldPosition();
    virtual void                update(const float delta, const DirectX::XMFLOAT4X4 &pTransform, const bool parentUpdated);
    virtual void                draw() {}
    virtual                     ~WorldObject() {}
};