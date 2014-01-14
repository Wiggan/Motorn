#pragma once
#include <d3d11.h>
#include "Structs.h"
#include "Component.h"
class PointLightComponent : public Component {
    PointLight          mPointLight;
public:
                PointLightComponent(const std::string &pName);
                ~PointLightComponent();
    const       PointLight& getPointLight();
    void        setAmbient(const DirectX::XMFLOAT4 &pAmbient);
    void        setSpecular(const DirectX::XMFLOAT4 &pSpecular);
    void        setDiffuse(const DirectX::XMFLOAT4 &pDiffuse);
};

