#include "TestGame.h"
#include "ResourceLoader.h"

TestGame::TestGame() {
}


TestGame::~TestGame() {
}

void TestGame::setupWorld() {
	using namespace DirectX;
	mWorld = Entity("World");
	std::vector<Texture*> textures;
	textures.push_back(ResourceLoader::getTexture("bark"));
	Entity* child1 = new Entity("Child1");
	MeshResource* boxMesh = ResourceLoader::getMeshResource("box3");

	child1->addComponent(new Mesh(mStuff, boxMesh, textures, ResourceLoader::getMaterialResource("box3")));
	//Entity child2("Child2"ResourceLoader::getMesh("box3", textures, ResourceLoader::getMaterial("box3")));
	//Entity child3 = Entity("Child3", ResourceLoader::getMesh("box3", textures, ResourceLoader::getMaterial("box3")));
	//Entity* shroom = new Entity("Shroom", ResourceLoader::getMesh("shroom", textures, ResourceLoader::getMaterial("box3")));
	//shroom->setPosition(XMFLOAT3(7.0f, 2.0f, 0.0f));
	mWorld.addEntity(child1);

	//Entity* sprite = new Entity("Sprite", ResourceLoader::getSprite(ResourceLoader::getTexture("bark")));
	//sprite->setPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//sprite->setAlwaysFacingCamera(true);
	//mWorld.add(*sprite);
}

int main(int argc, char* argv[]) {
	TestGame game;
	game.startGame();
	return 0;
}