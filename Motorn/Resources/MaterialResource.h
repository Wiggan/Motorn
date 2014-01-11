#pragma once
#include "Structs.h"
#include <iostream>
#include <vector>
class MaterialResource;
class MaterialResourceListener {
public:
    virtual void onMaterialLoaded(MaterialResource* pUpdatedMaterialResource) = 0;
};
class MaterialResource {
    Material                                mMaterial;
    std::vector<MaterialResourceListener*>    mListeners;
    std::string                                mFileName;
public:
                        MaterialResource(const std::string pFileName);
                        ~MaterialResource();
    Material*            getMaterial();
    bool                load();
    bool                reload();
    void                addListener(MaterialResourceListener* pListener);
};

