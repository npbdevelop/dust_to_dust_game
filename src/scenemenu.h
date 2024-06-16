#ifndef SCENEMENU_H
#define SCENEMENU_H

#include "scene.h"
#include <map>
#include <memory>
#include <deque>
#include "assets.h"
#include "entitymanager.h"
#include "game_engine.h"

class SceneMenu: public Scene
{
protected:

    std::string mTitle;
    std::vector<std::string> mMenuStrings;
    std::vector<std::string> mLevelPaths;
    sf::Text mMenuText;
    size_t mSelectedMenuIndex { 0 };
    std::string mMenuPath;

    sf::Text mOption1;
    sf::Text mOption2;

    bool mDrawTextures { true };
    bool mDrawCollision { false }; // Debug bools
    bool mDrawGrid { false };

    void init();
    void update() override;
    void onEnd() override;
    void sDoAction(const Action& action) override;

public:

    SceneMenu() = default;
    SceneMenu(GameEngine* gameEngine);

    ~SceneMenu() = default;

    void loadMenu(const std::string& menuPath);
    void sRender() override;
};

#endif