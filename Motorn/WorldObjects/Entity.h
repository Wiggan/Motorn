#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "WorldObject.h"
#include "Component.h"
#include <vector>

class Entity : public WorldObject {
private:
	std::vector<WorldObject*> mChildren;
	bool					mUpdate;
	Entity*					mParent;
	std::string 			mName;
public:
							Entity(const std::string &pName);
							Entity() : Entity("") {}
							~Entity();
	void					addComponent(Component* pComponent);
	void					addEntity(Entity* pEntity);
	virtual void			update(const float delta, const DirectX::XMFLOAT4X4 &pTransform, const bool parentUpdated);
	virtual void			draw();
	void					setPosition(const DirectX::XMFLOAT3 &pPosition);
	void					setRotation(const DirectX::XMFLOAT3 &pRotation);
	void					setScale(const DirectX::XMFLOAT3 &pScale);
	void					setParent(Entity* pParent);
	DirectX::XMFLOAT4X4&	getTransform();
};

