#include "LevelResource.h"
#include <fstream>
#include <sstream>
#include "ResourceLoader.h"

LevelResource::LevelResource(const D3dStuff &pStuff, const std::string pFileName) : mFileName(pFileName), mStuff(pStuff) {
}
bool LevelResource::reload() {
    if ( load() ) {
        mListener->onLevelLoaded(this);
        return true;
    } else {
        std::cout << "Failed reloading mesh resource!" << std::endl;
        return false;
    }
}
bool LevelResource::load() {
    using namespace std;
    tinyxml2::XMLDocument level;
    level.LoadFile(mFileName.c_str());
    if ( level.ErrorID() == 0 ) {
        mWorld = createEntity(level.FirstChildElement("entity"));
        mDirectionalLight = createDirectionalLight(level.FirstChildElement("entity")->FirstChildElement("directionalLight"));
    } else {
        cout << "Failed loading level! " << level.ErrorID() << endl;
        return false;
    }
    return true;
}
LevelResource::~LevelResource() {
}
Entity* LevelResource::getLevel() {
    return mWorld;
}
void LevelResource::addListener(LevelResourceListener* pListener) {
    mListener = pListener;
}
Entity* LevelResource::createEntity(const tinyxml2::XMLNode* node) {
    using namespace tinyxml2;
    using namespace DirectX;
    Entity* entity = new Entity(node->FirstChildElement("name")->GetText());
    fillEntity(node, entity);
    return entity;
}
AIEntity* LevelResource::createAIEntity(const tinyxml2::XMLNode* node) {
    using namespace tinyxml2;
    using namespace DirectX;
    AIEntity* entity = new AIEntity(node->FirstChildElement("name")->GetText());
    fillEntity(node, entity);
    for ( const XMLElement* child = node->FirstChildElement(); child != NULL; child = child->NextSiblingElement() ) {
        if ( strcmp("point", child->Name()) == 0 ) {
            entity->addWayPoint(createVector(child));
        }
    }
    return entity;
}
ScopeEntity* LevelResource::createScopeEntity(const tinyxml2::XMLNode* node) {
    using namespace tinyxml2;
    using namespace DirectX;
    ScopeEntity* entity = new ScopeEntity(node->FirstChildElement("name")->GetText());
    fillEntity(node, entity);
    for ( const XMLElement* child = node->FirstChildElement(); child != NULL; child = child->NextSiblingElement() ) {
        if ( strcmp("mesh", child->Name()) == 0 && strcmp("ray", child->FirstChildElement("name")->GetText()) == 0 ) {
            entity->setRayComponent(createMesh(child));
        } 
    }
    return entity;
}
PointLightComponent* LevelResource::createPointLight(const tinyxml2::XMLNode* node) {
    using namespace tinyxml2;
    using namespace DirectX;
    const XMLElement* positionNode = node->FirstChildElement("position");
    const XMLElement* ambientNode = node->FirstChildElement("ambient");
    const XMLElement* diffuseNode = node->FirstChildElement("diffuse");
    const XMLElement* specularNode = node->FirstChildElement("specular");
    PointLightComponent* light = new PointLightComponent(node->FirstChildElement("name")->GetText());
    if ( positionNode != NULL ) {
        light->setPosition(createVector(positionNode));
    }
    if ( ambientNode != NULL ) {
        light->setAmbient(createColor(ambientNode));
    }
    if ( diffuseNode != NULL ) {
        light->setDiffuse(createColor(diffuseNode));
    }
    if ( specularNode != NULL ) {
        light->setSpecular(createColor(specularNode));
    }
    mPointLights.push_back(light);
    return light;
}
DirectionalLight* LevelResource::createDirectionalLight(const tinyxml2::XMLNode* node) {
    using namespace tinyxml2;
    using namespace DirectX;
    DirectionalLight* light = new DirectionalLight();
    if ( node != NULL ) {
        const XMLElement* directionNode = node->FirstChildElement("direction");
        const XMLElement* ambientNode = node->FirstChildElement("ambient");
        const XMLElement* diffuseNode = node->FirstChildElement("diffuse");
        const XMLElement* specularNode = node->FirstChildElement("specular");
        if ( directionNode != NULL ) {
            light->direction =createVector(directionNode);
        }
        if ( ambientNode != NULL ) {
            light->ambient = createColor(ambientNode);
        }
        if ( diffuseNode != NULL ) {
            light->diffuse = createColor(diffuseNode);
        }
        if ( specularNode != NULL ) {
            light->specular =createColor(specularNode);
        }
    }
    return light;
}
Mesh* LevelResource::createMesh(const tinyxml2::XMLNode* node) {
    using namespace tinyxml2;
    std::vector<Texture*> textures;
    textures.push_back(ResourceLoader::getTexture(node->FirstChildElement("texture")->GetText()));
    Mesh* mesh = new Mesh(node->FirstChildElement("name")->GetText(), mStuff,
        ResourceLoader::getMeshResource(node->FirstChildElement("fileName")->GetText()), textures,
        ResourceLoader::getMaterialResource(node->FirstChildElement("material")->GetText()));
    fillWorldObject(node, mesh);
    return mesh;
}
ScopeComponent* LevelResource::createScopeComponent(const tinyxml2::XMLNode* node) {
    using namespace tinyxml2;
    ScopeComponent* scope = new ScopeComponent(node->FirstChildElement("name")->GetText(), mStuff);
    fillWorldObject(node, scope);
    return scope;
}
void LevelResource::fillEntity(const tinyxml2::XMLNode* node, Entity* entity) {
    using namespace tinyxml2;
    using namespace DirectX;
    fillWorldObject(node, entity);
    for ( const XMLElement* child = node->FirstChildElement(); child != NULL; child = child->NextSiblingElement() ) {
        if ( strcmp("entity", child->Name()) == 0 ) {
            entity->addEntity(createEntity(child));
        } else if ( strcmp("mesh", child->Name()) == 0 ) {
            entity->addComponent(createMesh(child));
        } else if ( strcmp("pointLight", child->Name()) == 0 ) {
            entity->addComponent(createPointLight(child));
        } else if ( strcmp("scopeEntity", child->Name()) == 0 ) {
            entity->addEntity(createScopeEntity(child));
        } else if ( strcmp("aiEntity", child->Name()) == 0 ) {
            entity->addEntity(createAIEntity(child));
        } else if ( strcmp("scopeComponent", child->Name()) == 0 ) {
            entity->addComponent(createScopeComponent(child));
        }
    }
}
void LevelResource::fillWorldObject(const tinyxml2::XMLNode* node, WorldObject* obj) {
    using namespace tinyxml2;
    using namespace DirectX;
    const XMLElement* positionNode = node->FirstChildElement("position");
    const XMLElement* rotationNode = node->FirstChildElement("rotation");
    const XMLElement* scaleNode = node->FirstChildElement("scale");
    if ( positionNode != NULL ) {
        obj->setPosition(createVector(positionNode));
    }
    if ( rotationNode != NULL ) {
        obj->setRotation(createVector(rotationNode));
    }
    if ( scaleNode != NULL ) {
        obj->setScale(createVector(scaleNode));
    }
}
DirectX::XMFLOAT3 LevelResource::createVector(const tinyxml2::XMLElement* element) {
    float x;
    float y;
    float z;
    element->QueryFloatAttribute("x", &x);
    element->QueryFloatAttribute("y", &y);
    element->QueryFloatAttribute("z", &z);
    return DirectX::XMFLOAT3(x, y, z);
}
DirectX::XMFLOAT4 LevelResource::createColor(const tinyxml2::XMLElement* element) {
    float r;
    float g;
    float b;
    float a;
    element->QueryFloatAttribute("r", &r);
    element->QueryFloatAttribute("g", &g);
    element->QueryFloatAttribute("b", &b);
    element->QueryFloatAttribute("a", &a);
    return DirectX::XMFLOAT4(r, g, b, a);
}
std::vector<PointLightComponent*> LevelResource::getPointLights() {
    return mPointLights;
}
DirectionalLight* LevelResource::getDirectionalLight() {
    return mDirectionalLight;
}