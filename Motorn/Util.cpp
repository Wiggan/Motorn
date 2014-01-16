#include "Util.h"
#include <iostream>
using namespace DirectX;

XMFLOAT3 Util::forward = XMFLOAT3(0.0f, 0.0f, 1.0f);
XMFLOAT3 Util::up = XMFLOAT3(0.0f, 1.0f, 1.0f);
XMFLOAT3 Util::right = XMFLOAT3(1.0f, 0.0f, 0.0f);
DirectX::XMFLOAT3 Util::lerp(const DirectX::XMFLOAT3& pStart, const DirectX::XMFLOAT3& pEnd, float pProgress) {
    XMFLOAT3 result;
    XMStoreFloat3(&result, XMVectorLerp(XMLoadFloat3(&pStart), XMLoadFloat3(&pEnd), pProgress));
    return result;
}
DirectX::XMFLOAT3 Util::lerp(const std::vector<DirectX::XMFLOAT3>& pList, float pProgress, bool pLoop) {
    XMFLOAT3 result;
    int listSize = pList.size() -1 ;
    int startPoint = (int)(listSize * pProgress);
    float progress = pProgress*listSize - startPoint;
    //std::cout << "Startpoint: " << startPoint << " progress: " << progress << std::endl;
    return lerp(pList[startPoint], pList[startPoint + 1], progress);
}
DirectX::XMFLOAT3 Util::bez(const DirectX::XMFLOAT3& p0, const DirectX::XMFLOAT3& p1, const DirectX::XMFLOAT3& p2, float pProgress) {
    XMFLOAT3 result;
    XMStoreFloat3(&result, XMVectorLerp(XMVectorLerp(XMLoadFloat3(&p0), XMLoadFloat3(&p1), pProgress), 
        XMVectorLerp(XMLoadFloat3(&p1), XMLoadFloat3(&p2), pProgress), pProgress));
    return result;
}
DirectX::XMFLOAT3 Util::bez(const std::vector<DirectX::XMFLOAT3>& pList, float pProgress, bool pLoop) {
    assert(pList.size() % 2 == 1);
    XMFLOAT3 result;
    int listSize = (pList.size() - 1)/2;
    int startPoint = (int)(listSize * pProgress);
    float progress = pProgress*listSize - startPoint;
    //std::cout << "Startpoint: " << startPoint << " progress: " << progress << std::endl;
    return bez(pList[startPoint * 2], pList[startPoint * 2 + 1], pList[startPoint * 2 + 2], progress);
}

DirectX::XMFLOAT3 Util::dir2rot(const DirectX::XMFLOAT3& pDirection) {
    float dx = pDirection.x;
    float dy = pDirection.y;
    float dz = pDirection.z;
    float yaw = -atan2(dx, -dz) + 3.14159265359;
    float pitch = -atan2(dy, sqrt(dx*dx + dz*dz));
    XMFLOAT3 result( pitch, yaw, 0.0f);
    return result;
}
DirectX::XMFLOAT3 Util::dir(const DirectX::XMFLOAT3& pStart, const DirectX::XMFLOAT3& pEnd) {
    XMFLOAT3 direction;
    XMStoreFloat3(&direction, XMVector3Normalize(XMLoadFloat3(&pEnd) - XMLoadFloat3(&pStart)));
    return direction;
}