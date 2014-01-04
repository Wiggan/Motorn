#include "TestGame.h"
#include "ResourceLoader.h"

TestGame::TestGame() {
}


TestGame::~TestGame() {
}

void TestGame::setupWorld() {
	using namespace DirectX;
	mWorld = Entity("World", NULL);
	Entity* child1 = new Entity("Child1", ResourceLoader::getMesh("box"));
	Entity* child2 = new Entity("Child2", ResourceLoader::getMesh("box"));
	Entity* child3 = new Entity("Child3", ResourceLoader::getMesh("box"));
	child2->setScale(XMFLOAT3(0.3f, 0.3f, 0.3f));
	child2->setPosition(XMFLOAT3(0.0f, 3.0f, 0.0f));
	child3->setScale(XMFLOAT3(0.3f, 0.3f, 0.3f));
	child3->setPosition(XMFLOAT3(0.0f, 3.0f, 0.0f));
	child1->add(*child2);
	child2->add(*child3);
	mWorld.add(*child1);
}

int main(int argc, char* argv[]) {
	TestGame game;
	game.startGame();
	return 0;
}