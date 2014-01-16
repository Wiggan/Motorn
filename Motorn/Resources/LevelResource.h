#pragma once
#include "Entity.h"
#include "ScopeComponent.h"
#include "ScopeEntity.h"
#include "Mesh.h"
#include "PointLightComponent.h"
#include "Structs.h"
#include <iostream>
#include "tinyxml2.h"
#include "AIEntity.h"
class LevelResource;
class LevelResourceListener {
public:
    virtual void onLevelLoaded(LevelResource* pUpdatedLevelResource) = 0;
};
class LevelResource {
private:
    Entity*                             mWorld;
    std::string                         mFileName;
    LevelResourceListener*              mListener;
    Entity*                             createEntity(const tinyxml2::XMLNode* node);
    ScopeEntity*                        createScopeEntity(const tinyxml2::XMLNode* node);
    AIEntity*                           createAIEntity(const tinyxml2::XMLNode* node);
    Mesh*                               createMesh(const tinyxml2::XMLNode* node);
    ScopeComponent*                     createScopeComponent(const tinyxml2::XMLNode* node);
    PointLightComponent*                createPointLight(const tinyxml2::XMLNode* node);
    DirectionalLight*                   createDirectionalLight(const tinyxml2::XMLNode* node);
    DirectX::XMFLOAT3                   createVector(const tinyxml2::XMLElement* node);
    DirectX::XMFLOAT4                   createColor(const tinyxml2::XMLElement* node);
    void                                fillWorldObject(const tinyxml2::XMLNode* node, WorldObject* obj);
    void                                fillEntity(const tinyxml2::XMLNode* node, Entity* obj);
    D3dStuff                            mStuff;
    std::vector<PointLightComponent*>   mPointLights;
    std::vector<Entity*>                mRayCastableEntities;
    DirectionalLight*                   mDirectionalLight;
public:
                                        LevelResource(const D3dStuff &pStuff, const std::string pFileName);
                                        ~LevelResource();
    Entity*                             getLevel();
    std::vector<PointLightComponent*>   getPointLights();
    std::vector<Entity*>                getRayCastableEntities();
    DirectionalLight*                   getDirectionalLight();
    bool                                load();
    bool                                reload();
    void                                addListener(LevelResourceListener* pListener);
};

