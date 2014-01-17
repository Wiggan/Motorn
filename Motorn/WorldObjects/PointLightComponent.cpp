#include "PointLightComponent.h"


PointLightComponent::PointLightComponent(const std::string &pName) : Component(pName) {
    mPointLight.ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
    mPointLight.specular = { 0.2f, 0.2f, 0.2f, 1.0f };
    mPointLight.diffuse = { 0.1f, 0.1f, 0.1f, 1.0f };
    mPointLight.attenuation = { 0.0f, 0.1f, 0.0f };
    mPointLight.range = 60.0f;
    mPointLight.position = { 0.0f, 0.0f, 0.0f };
}


PointLightComponent::~PointLightComponent() {
}

const PointLight& PointLightComponent::getPointLight() {
    mPointLight.position = getWorldPosition();
    return mPointLight;
}
void PointLightComponent::setAmbient(const DirectX::XMFLOAT4 &pAmbient) {
    mPointLight.ambient = pAmbient;
}
void PointLightComponent::setDiffuse(const DirectX::XMFLOAT4 &pDiffuse) {
    mPointLight.diffuse = pDiffuse;
}
void PointLightComponent::setSpecular(const DirectX::XMFLOAT4 &pSpecular) {
    mPointLight.specular = pSpecular;
}
void PointLightComponent::setAttenuation(const DirectX::XMFLOAT3 &pAttenuation) {
    mPointLight.attenuation = pAttenuation;
}
void PointLightComponent::setRange(float pRange) {
    mPointLight.range = pRange;
}