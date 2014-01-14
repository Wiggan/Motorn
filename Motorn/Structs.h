#pragma once
#include <D3D11.h>
#include <DirectXMath.h>
struct D3dStuff {
    ID3D11Device *dev;                     // the pointer to our Direct3D device interface
    ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
};
struct Material {
    Material() {
        ZeroMemory(this, sizeof(this));
    }
    DirectX::XMFLOAT4 ambient;
    DirectX::XMFLOAT4 diffuse;
    DirectX::XMFLOAT4 specular; // w = power, or shininess
};
struct DirectionalLight {
    DirectionalLight() {
        ZeroMemory(this, sizeof(this));
    }
    DirectX::XMFLOAT4 ambient;
    DirectX::XMFLOAT4 diffuse;
    DirectX::XMFLOAT4 specular;
    DirectX::XMFLOAT3 direction;
    float pad;
};
struct PointLight {
    PointLight() {
        ZeroMemory(this, sizeof(this));
    }
    DirectX::XMFLOAT4 ambient;
    DirectX::XMFLOAT4 diffuse;
    DirectX::XMFLOAT4 specular;
    DirectX::XMFLOAT3 position;
    float pad;
    DirectX::XMFLOAT3 attenuation;
    float range;
};
struct PerFrameBuffer {
    PerFrameBuffer() {
        ZeroMemory(this, sizeof(this));
    }
    DirectX::XMFLOAT4X4     viewMatrix;

    DirectX::XMFLOAT4X4     projectionMatrix;

    DirectionalLight        directionalLight;

    PointLight              pointLights[3];

    DirectX::XMFLOAT3       cameraPosition;
    float                   fogStart;

    double                  time;
    int                     pointLightCount;
    float                   fogRange;

    DirectX::XMFLOAT4       fogColor;
};
struct PerObjectBuffer {
    PerObjectBuffer() {
        ZeroMemory(this, sizeof(this));
    }
    DirectX::XMFLOAT4X4 worldMatrix;
    DirectX::XMFLOAT4X4 worldMatrixInverseTranspose;
    Material material;
};
