#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "Sprite.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <stdlib.h> 
#include "../Camera.h"

Mesh::Mesh(D3dStuff &stuff, MeshResource* pMeshResource, std::vector<Texture*> pTextures, MaterialResource* pMaterialResource) {
    using namespace DirectX;
    using namespace std;
    dev = stuff.dev;
    devcon = stuff.devcon;
    mTextures = pTextures;
    for ( auto it = pTextures.begin(); it != pTextures.end(); it++ ) {
        mTextureViews.push_back((*it)->getTextureView());
    }
    pMaterialResource->addListener(this);
    onMaterialLoaded(pMaterialResource);
    pMeshResource->addListener(this);
    onMeshLoaded(pMeshResource);
}
Mesh::~Mesh()
{
}
void Mesh::draw() {
    using namespace DirectX;
    constants.worldMatrix = mTransform;
    XMMATRIX A = XMLoadFloat4x4(&mTransform);
    A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    XMVECTOR det = XMMatrixDeterminant(A);
    XMStoreFloat4x4(&constants.worldMatrixInverseTranspose, XMMatrixInverse(&det, A));
    constants.material = *mMaterial;
    setShaderConstants(constants);
    UINT stride = sizeof(VERTEX);
    UINT offset = 0;
    devcon->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
    devcon->IASetIndexBuffer( mIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );
    devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    devcon->PSSetShaderResources(0, 1, &mTextureViews[0]);
    devcon->DrawIndexed(mIndexCount, 0, 0);
}
void Mesh::onMeshLoaded(MeshResource* pUpdatedMeshResource) {
    mVertexBuffer = pUpdatedMeshResource->getVertexBuffer();
    mIndexBuffer = pUpdatedMeshResource->getIndexBuffer();
    mIndexCount = pUpdatedMeshResource->getIndexCount();
}
void Mesh::onMaterialLoaded(MaterialResource* pUpdatedMaterialResource) {
    mMaterial = pUpdatedMaterialResource->getMaterial();
}