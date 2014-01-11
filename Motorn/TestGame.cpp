#include "TestGame.h"
#include "ResourceLoader.h"
#include "ScopeComponent.h"

TestGame::TestGame() {
}
TestGame::~TestGame() {
}
void TestGame::setupWorld() {
    using namespace DirectX;
    mWorld = Entity("World");
    std::vector<Texture*> textures;
    textures.push_back(ResourceLoader::getTexture("bark"));
    monkey = new AIEntity("monkey");
    movingMonkey = new ScopeEntity("movingMonkey");
    MeshResource* boxMesh = ResourceLoader::getMeshResource("box3");
    MeshResource* lineMesh = ResourceLoader::getMeshResource("line");
    monkey->addComponent(new Mesh(mStuff, boxMesh, textures, ResourceLoader::getMaterialResource("box3")));
    monkey->setPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 6.0f));
    movingMonkey->setPosition(DirectX::XMFLOAT3(0.0f, 0.0f, -6.0f));
    movingMonkey->addComponent(new Mesh(mStuff, boxMesh, textures, ResourceLoader::getMaterialResource("box3")));
    movingMonkey->addComponent(new ScopeComponent(mStuff));
    movingMonkey->setRayComponent(new Mesh(mStuff, lineMesh, textures, ResourceLoader::getMaterialResource("line")));


    mRayCastableEntities.push_back(monkey);

    mWorld.addEntity(monkey);
    mWorld.addEntity(movingMonkey);

}
void TestGame::update(const double delta) {
    static double total = delta;
    total += delta;
    //movingMonkey->setRotation(DirectX::XMFLOAT3(total / 10000, 0.0f, 0.0f));
    mWorld.update(delta, mWorldTransform, false);
}
int main(int argc, char* argv[]) {
    TestGame game;
    game.startGame();
    return 0;
}