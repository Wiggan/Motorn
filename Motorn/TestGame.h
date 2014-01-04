#pragma once
#include "AbstractGame.h"

class TestGame : public AbstractGame {
public:
	virtual void setupWorld();
	TestGame();
	~TestGame();
};

