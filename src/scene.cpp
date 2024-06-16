#include "scene.h"
#include "game_engine.h"

Scene::Scene(GameEngine* game)
    : mGame { game }
{}


void Scene::simulate(const int frames)
{
    // Calls current scene's update() method a given number of times
    for (int i { 0 }; i < frames; ++i)
    {

    }

}

void Scene::registerAction(int inputKey, const std::string& actionName)
{
    mActionMap.insert({ inputKey, actionName });
}

/*
During positioning enity has already had origin cet to center, so this method returns the position the center
of a specific size entity needs to be at to have its bottom left corner align properly with grid square 
(gridX, gridY) upon positioning
*/
Vector Scene::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
    Vector size = entity->getComponent<CAnimation>().animation.getSize();
    Vector center = { gridX * mGridSize.x + size.x / 2, gridY * mGridSize.y + size.y / 2 };
    return center;
}

void Scene::doAction(Action action)
{
    sDoAction(action);
}

int Scene::currentFrame() const
{
    return mCurrentFrame;
}

bool Scene::hasEnded() const
{
    return mHasEnded;
}

const ActionMap& Scene::getActionMap() const
{
    return mActionMap;
}

void Scene::drawLine(const Vector& p1, const Vector& p2)
{
    sf::Vertex line[] = { sf::Vertex { { p1.x, p1.y } }, sf::Vertex { { p2.x, p2.y } } };
    mGame->window().draw(line, 2, sf::Lines);
}

float Scene::height()
{
    return mGame->window().getSize().y;
}

float Scene::width()
{
    return mGame->window().getSize().x;
}

void Scene::setPaused()
{
    mPaused = !mPaused;
}
