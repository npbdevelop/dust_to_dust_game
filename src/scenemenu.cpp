/*
Scene file for the main menu of the game
*/

#include "scenemenu.h"

SceneMenu::SceneMenu(GameEngine* gameEngine)
    : Scene { gameEngine }, mMenuPath { "config/menu.txt" }, mTitle { "Game Prototype" }
{   
    // This constructor gets called by GameEngine, like with all scenes. First one
    init();
}

void SceneMenu::init()
{
    // Register menu actions with base Scene class
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");

    registerAction(sf::Keyboard::W, "UP");
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::Space, "SELECT");

    int titleSize { 256 };

    mMenuText.setString(mTitle);
    mMenuText.setFont(mGame->assets().getFont("Robot"));
    mMenuText.setCharacterSize(titleSize);
    mMenuText.setFillColor(sf::Color::Black);
    mMenuText.setOrigin(mMenuText.getLocalBounds().width / 2, mMenuText.getLocalBounds().height / 2);
    mMenuText.setPosition(width() / 2, 256);

    mMenuStrings = { "Play", "Quit" };
    mOption1.setString(mMenuStrings[0]);
    mOption2.setString(mMenuStrings[1]);
    mOption1.setFont(mGame->assets().getFont("Robot"));
    mOption1.setCharacterSize(128);
    mOption1.setFillColor(sf::Color::Black);
    mOption1.setOrigin(mOption1.getLocalBounds().width / 2, mOption1.getLocalBounds().height / 2);
    mOption1.setPosition(width() / 6, 640);
    mOption2.setFont(mGame->assets().getFont("Robot"));
    mOption2.setCharacterSize(128);
    mOption2.setFillColor(sf::Color::Black);
    mOption2.setOrigin(mOption2.getLocalBounds().width / 2, mOption2.getLocalBounds().height / 2);
    mOption2.setPosition(width() / 6, 896);

    mGridText.setFont(mGame->assets().getFont("Sans"));
    mGridText.setCharacterSize(24);

    loadMenu(mMenuPath);
}

void SceneMenu::loadMenu(const std::string& menuPath)
{
    mEntityManager = EntityManager(); // Call constructor to reset

    std::ifstream inFile { mMenuPath };

    std::string lines {};
    std::string type {};
    std::string name {};
    float i {};
    float j {};

    // Parse every line and add entities with the correct parameters
    while (std::getline(inFile, lines))
    {
        std::stringstream line { lines };
        line >> type >> name >> i >> j;

        auto entity = mEntityManager.addEntity(type);
        entity->addComponent<CAnimation>(mGame->assets().getAnimation(name), true);
        Vector midPos { gridToMidPixel(i, j, entity) }; // Correct position relative to sprite center points
        entity->addComponent<CTransform>(midPos);
        entity->getComponent<CAnimation>().animation.getSprite().setPosition(midPos.x, midPos.y);
    }

    // Leaving scope right away, no need to close file

}

void SceneMenu::update()
{
    mEntityManager.update();
    sRender();
}

/*
This is how actions are performed for the menu scene. The arbitration here implies that it is very easy to
create replay files if we do it this way, since we could just read in a file of strings where the strings
are all of the inputs and feed it into the game engine
*/

void SceneMenu::sDoAction(const Action& action)
{
    if (action.type() == "START")
    {
        if (action.name() == "UP")
        {
            if (mSelectedMenuIndex > 0)
            {
                mSelectedMenuIndex--;
            }
            else
            {
                mSelectedMenuIndex = mMenuStrings.size() - 1;
            }
        }
        else if (action.name() == "DOWN")
        {
            mSelectedMenuIndex = (mSelectedMenuIndex + 1) % mMenuStrings.size();
        }
        else if (action.name() == "TOGGLE_GRID")
        {
            mDrawGrid = !mDrawGrid;
        }
        else if (action.name() == "SELECT")
        {
            if (mSelectedMenuIndex == 0)
            {
                mGame->changeScene("PLAY", std::make_shared<ScenePlay>(mGame, "play.txt"));
            }
            if (mSelectedMenuIndex == 1)
            {
                onEnd();
            }
        }
        else if (action.name() == "QUIT")
        {
            onEnd();
        }
    }
}

void SceneMenu::sRender()
{
    // Clear window white
    mGame->window().clear(sf::Color::White);

    sf::View view = mGame->window().getDefaultView();
    mGame->window().setView(view);

    // Draw all Entity textures and animations
    if (mDrawTextures)
    {
        for (auto& e : mEntityManager.getEntities())
        {
            auto& transform = e->getComponent<CTransform>();
            if (e->hasComponent<CAnimation>())
            {
                auto& animation = e->getComponent<CAnimation>().animation;
                mGame->window().draw(animation.getSprite());
            }
        }
    }

    if (mDrawGrid)
    {
        float leftX = 0;
        float rightX = width();
        float nextGridX = leftX + mGridSize.x;

        for (float x { nextGridX }; x < rightX; x += mGridSize.x)
        {
            drawLine(Vector(x, 0), Vector(x, height()));
        }

        for (float y { 0 }; y < height(); y += mGridSize.y)
        {
            drawLine(Vector(leftX, height() - y), Vector(rightX, height() - y));

            for (float x { 0 }; x < rightX; x += mGridSize.x)
            {
                std::string xCell = std::to_string((int)x / (int)mGridSize.x);
                std::string yCell = std::to_string((int)y / (int)mGridSize.y);
                mGridText.setString("(" + xCell + ", " + yCell + ")");
                mGridText.setPosition(x + 3, height() - y - mGridSize.y + 2);
                mGame->window().draw(mGridText);
            }
        }
    }

    /*
    Need to incorporate fonts as entities
    */

    if (mSelectedMenuIndex == 0)
    {
        mOption1.setFillColor(sf::Color::White);
        mOption2.setFillColor(sf::Color::Black);
    }
    if (mSelectedMenuIndex == 1)
    {
        mOption1.setFillColor(sf::Color::Black);
        mOption2.setFillColor(sf::Color::White);
    }

    mGame->window().draw(mMenuText);
    mGame->window().draw(mOption1);
    mGame->window().draw(mOption2);

    mGame->window().display();
}

void SceneMenu::onEnd()
{
    mGame->quit();
}