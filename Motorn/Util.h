#pragma once
#include <DirectXMath.h>
#include <vector>
class Util {
public:
    static DirectX::XMFLOAT3 forward;
    static DirectX::XMFLOAT3 right;
    static DirectX::XMFLOAT3 up;
    static DirectX::XMFLOAT3 lerp(const DirectX::XMFLOAT3& pStart, const DirectX::XMFLOAT3& pEnd, float pProgress);
    static DirectX::XMFLOAT3 lerp(const std::vector<DirectX::XMFLOAT3>& pList, float pProgress, bool pLoop);
    static DirectX::XMFLOAT3 bez(const DirectX::XMFLOAT3& p0, const DirectX::XMFLOAT3& p1, const DirectX::XMFLOAT3& p2, float pProgress);
    static DirectX::XMFLOAT3 bez(const std::vector<DirectX::XMFLOAT3>& pList, float pProgress, bool pLoop);
    static DirectX::XMFLOAT3 dir2rot(const DirectX::XMFLOAT3& pDirection);
    static DirectX::XMFLOAT3 dir(const DirectX::XMFLOAT3& pStart, const DirectX::XMFLOAT3& pEnd);
};

