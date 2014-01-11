#pragma once
#include <string>
#include "Drawable.h"
#include "Structs.h"
#include "Texture.h"
#include "MaterialResource.h"
#include "MeshResource.h"
#include <vector>
class Mesh : public Drawable, public MeshResourceListener, public MaterialResourceListener
{
    ID3D11Buffer                            *mIndexBuffer;
    int                                        mIndexCount;
    std::vector<ID3D11ShaderResourceView*>    mTextureViews;
    std::vector<Texture*>                    mTextures;
    Material*                                mMaterial;
public:
    Mesh(D3dStuff &stuff, MeshResource* pMeshResource, std::vector<Texture*> pTextures, MaterialResource* pMaterialResource);
    ~Mesh();
    virtual void            draw();
    virtual void            onMeshLoaded(MeshResource* pUpdatedMeshResource);
    virtual void            onMaterialLoaded(MaterialResource* pUpdatedMaterialResource);
};

