#include "ScopeComponent.h"
#include "Entity.h"
#include <iostream>
ScopeComponent::ScopeComponent(const std::string &pName, D3dStuff &pStuff) : Component(pName) {
    dev = pStuff.dev;
    devcon = pStuff.devcon;
}
ScopeComponent::~ScopeComponent() {
}
void ScopeComponent::rayCast(const std::vector<Entity*> &pRayCastableEntities) {
    using namespace DirectX;
    for ( auto it = pRayCastableEntities.begin(); it != pRayCastableEntities.end(); it++ ) {
        if ( mOwner != *it ) {
            float distance;
            
            if ( (*it)->getBoundingSphere().Intersects(XMLoadFloat3(&getWorldPosition()), XMVector3Normalize(XMLoadFloat3(&getDirection())), distance) ) {
                mOwner->onRayCastHit(getWorldPosition(), getDirection(), distance, (*it));
               /* std::cout << "RayCast hit! Distance: " << distance << " from Position " << 
                    mPosition.x << ", " << mPosition.y << ", " << mPosition.z << ")" <<
                    ", Direction: (" << getDirection().x << ", " << getDirection().y << ", " << getDirection().z << ")" << std::endl;*/
            }
        }
    }
}
