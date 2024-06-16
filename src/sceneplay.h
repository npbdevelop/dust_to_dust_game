#ifndef SCENEPLAY_H
#define SCENEPLAY_H

#include "scene.h"
#include <map>
#include <memory>
#include <algorithm>
#include "assets.h"
#include "physics.h"
#include "entitymanager.h"
#include "game_engine.h"

class ScenePlay: public Scene
{
private:

    struct PlayerConfig
    {
        float X, Y, CX, SPEED, MAXSPEED, JUMP, GRAVITY;
        std::string WEAPON;
    };

protected:

    std::shared_ptr<Entity> mPlayer;
    std::string mLevelPath;
    PlayerConfig mPlayerConfig;
    bool mDrawTextures { true };
    bool mDrawCollision { false };
    bool mDrawGrid { false };
    Vector lastShot {};
    int coolDown {};

    void init();
    void update() override;
    void onEnd() override;
    void sDoAction(const Action& action) override;

public:

    ScenePlay() = default;
    ScenePlay(GameEngine* gameEngine, const std::string& levelPath);

    ~ScenePlay() = default;

    void loadLevel();
    void spawnPlayer();
    void spawnBullet(Vector& mousepos);
    void spawnEnemy();

    void sMovement();
    void sLifespan();
    void sCollision();
    void sAnimation();
    void sRender() override;

    void sEnemySpawner();
    void sDebug();

};

#endif