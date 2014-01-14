#pragma once
#include "AbstractGame.h"
#include "AIEntity.h"
#include "ScopeEntity.h"
#include "LevelResource.h"
class TestGame : public AbstractGame, public LevelResourceListener {
    Entity* monkey;
    ScopeEntity* movingMonkey;
    LevelResource* mLevelResource;
public:
    virtual void setupWorld();
    virtual void update(const double delta);
    TestGame();
    ~TestGame();
    virtual void onLevelLoaded(LevelResource* pUpdatedLevelResource);
};

