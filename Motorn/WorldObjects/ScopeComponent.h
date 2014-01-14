#pragma once
#include "Component.h"
#include "Structs.h"
#include "PrimitiveBatch.h"
#include "VertexTypes.h"
#include "Effects.h"
class ScopeComponent : public Component {
private:
    ID3D11Device                                            *dev;
    ID3D11DeviceContext                                     *devcon;
public:
                        ScopeComponent(const std::string &pName, D3dStuff &pStuff);
                        ~ScopeComponent();
    virtual void        rayCast(const std::vector<Entity*> &pRayCastableEntities);
};

