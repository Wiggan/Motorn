#pragma once
#include <d3d11.h>
#include "../Structs.h"
class PointLightResource {
	PointLight mPointLight;
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mDiffuse;
public:
	PointLightResource();
	~PointLightResource();
	const PointLight& getPointLight();
	void setPosition(const DirectX::XMFLOAT3 &pPosition);
	void setAmbient(const DirectX::XMFLOAT4 &pAmbient);
	void setSpecular(const DirectX::XMFLOAT4 &pSpecular);
	void setDiffuse(const DirectX::XMFLOAT4 &pDiffuse);
};

