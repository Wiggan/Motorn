#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Drawable.h"
#include <vector>

class Entity {
private:
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mRotation;
	DirectX::XMFLOAT3 mScale;
	DirectX::XMFLOAT4X4 mTransform;
	DirectX::XMFLOAT4X4 mParentTransform;
	Drawable* mDrawable;
	std::vector<Entity*> mChildren;
	Entity* mParent;
	bool mUpdate;
	bool mParentUpdate;
	std::string mName;
public:
	Entity(const std::string &pName, Drawable* pDrawable);
	Entity() : Entity("", NULL) {}
	~Entity();
	void add(Entity &pChild);
	void setParent(Entity &pParent);
	void update(const double pDelta);
	void draw();
	void setPosition(const DirectX::XMFLOAT3 &pPosition);
	void setRotation(const DirectX::XMFLOAT3 &pRotation);
	void setScale(const DirectX::XMFLOAT3 &pScale);
	void setParentTransform(const DirectX::XMFLOAT4X4 &pParentTransform);
	DirectX::XMFLOAT4X4& getTransform();
};

