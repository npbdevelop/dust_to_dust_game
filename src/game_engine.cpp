/*
This file handle's all of the game's high level functionality; initialization, switching scenes, main
game loop, keeping it running/ending the game. All of the specific functionality will be handled in
individual scene files
*/

#include "game_engine.h"

GameEngine::GameEngine(const std::string& path)
    : mSceneMap { { "MENU", {} }, { "PLAY", {} } }
{
    init(path);
}

void GameEngine::init(const std::string& path)
{
    mAssets.loadFromFile(path);

    // 4K window
    mWindow.create(sf::VideoMode(4096, 2304), "Definitely Not Mario");
    mWindow.setFramerateLimit(60);

    changeScene("MENU", std::make_shared<SceneMenu>(this));
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
    return mSceneMap[mCurrentScene];
} 

void GameEngine::run()
{
    // Start of main loop
    while (isRunning())
    {
        update();
    }
}

void GameEngine::update()
{
    currentScene()->update();
    sUserInput(); // Calls Scene::doAction(), sends action to current scene
}

bool GameEngine::isRunning()
{
    return mRunning & mWindow.isOpen();
}

sf::RenderWindow& GameEngine::window()
{
    return mWindow;
}

Assets& GameEngine::assets()
{
    return mAssets;
}

void GameEngine::sUserInput()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        // User requests to quit current scene
        if (event.type == sf::Event::Closed)
        {
            quit();
        }

        // For all other key presses
        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
        {
            // Skip this event if the current scene doesn't have an action associated with this key
            if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end())
            {
                continue;
            }
            // Determine start or end based on press or release
            const std::string actionType = (event.type == sf::Event::KeyPressed ? "START" : "END");

            // Look up the action and send it to the scene
            currentScene()->doAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));
        }
    }
} 

void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
    // If input specifies to end current scene, switch current scene name, this will change output of map
    mCurrentScene = sceneName;
    mSceneMap[mCurrentScene] = scene;
}

void GameEngine::quit()
{
    // Flag running as false to cut off main loop
    mRunning = false;
}
