#include "PointLightResource.h"


PointLightResource::PointLightResource() {
    mPointLight.ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
    mPointLight.specular = { 0.2f, 0.2f, 0.2f, 1.0f };
    mPointLight.diffuse = { 0.1f, 0.1f, 0.1f, 1.0f };
    mPointLight.attenuation = { 0.0f, 0.1f, 0.0f };
    mPointLight.range = 60.0f;
    mPointLight.position = { 0.0f, 0.0f, 0.0f };
}


PointLightResource::~PointLightResource() {
}

const PointLight& PointLightResource::getPointLight() {
    return mPointLight;
}
void PointLightResource::setPosition(const DirectX::XMFLOAT3 &pPosition) {
    mPointLight.position = pPosition;
}
void PointLightResource::setAmbient(const DirectX::XMFLOAT4 &pAmbient) {
    mPointLight.ambient = pAmbient;
}
void PointLightResource::setDiffuse(const DirectX::XMFLOAT4 &pDiffuse) {
    mPointLight.diffuse = pDiffuse;
}
void PointLightResource::setSpecular(const DirectX::XMFLOAT4 &pSpecular) {
    mPointLight.specular = pSpecular;
}