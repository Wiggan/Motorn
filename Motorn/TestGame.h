#pragma once
#include "AbstractGame.h"
#include "AIEntity.h"
#include "ScopeEntity.h"
class TestGame : public AbstractGame {
    AIEntity* monkey;
    ScopeEntity* movingMonkey;
public:
    virtual void setupWorld();
    virtual void update(const double delta);
    TestGame();
    ~TestGame();
};

