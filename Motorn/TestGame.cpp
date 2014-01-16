#include "TestGame.h"
#include "ResourceLoader.h"
#include "ScopeComponent.h"
#include "Camera.h"
TestGame::TestGame() {
}
TestGame::~TestGame() {
}
void TestGame::setupWorld() {
    using namespace DirectX;
    std::string level = "..\\assets\\levels\\level01.xml";
    mLevelResource = new LevelResource(mStuff, level);
    ResourceLoader::watchLevel(level);
    mLevelResource->addListener(this);
    mLevelResource->load();
    mWorld = mLevelResource->getLevel();
    mPointLights = mLevelResource->getPointLights();
    mDirectionalLight = mLevelResource->getDirectionalLight();
    mRayCastableEntities = mLevelResource->getRayCastableEntities();

    std::vector<Texture*> textures;
    textures.push_back(ResourceLoader::getTexture("bark"));
    monkey = new Entity("monkey");
    MeshResource* boxMesh = ResourceLoader::getMeshResource("box3");
    monkey->addComponent(new Mesh("camera", mStuff, boxMesh, textures, ResourceLoader::getMaterialResource("box3")));
    monkey->setPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
    //mWorld->addEntity(monkey);
}
void TestGame::update(const double delta) {
    static double total = delta;
    total += delta;
    mWorld->update(delta, mWorldTransform, false);
    monkey->setPosition(Camera::getInstance().getPosition());
    if ( ResourceLoader::isLevelChanged() ) {
        mLevelResource->reload();
    }
}
void TestGame::onLevelLoaded(LevelResource* pUpdatedLevelResource) {
    delete mWorld;
    ResourceLoader::reset();
    mRayCastableEntities = pUpdatedLevelResource->getRayCastableEntities();
    mWorld = pUpdatedLevelResource->getLevel();
    mPointLights = pUpdatedLevelResource->getPointLights();
    mDirectionalLight = pUpdatedLevelResource->getDirectionalLight();
}
int main(int argc, char* argv[]) {
    TestGame game;
    game.startGame();
    return 0;
}
