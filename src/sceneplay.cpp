#include "sceneplay.h"

/*
IMPLEMENT SOMEWHERE: if no collision, then in midair
*/

ScenePlay::ScenePlay(GameEngine* gameEngine, const std::string& levelPath)
    : Scene { gameEngine }, mLevelPath { "config/play.txt" }
{
    init();
}

void ScenePlay::init()
{
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");

    // REGISTER ALL OTHER GAMEPLAY ACTIONS

    registerAction(sf::Keyboard::W, "JUMP");
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::A, "LEFT");
    registerAction(sf::Keyboard::D, "RIGHT");
    registerAction(sf::Keyboard::Space, "SHOOT");

    mGridText.setCharacterSize(24);
    mGridText.setFont(mGame->assets().getFont("Sans"));

    loadLevel();
}

void ScenePlay::loadLevel()
{
    // Reset entity manager every time we load a level
    mEntityManager = EntityManager();

    std::ifstream inFile { mLevelPath };

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
        if (type == "Tile")
        {
            entity->addComponent<CBoundingBox>(entity->getComponent<CAnimation>().animation.getSize());
        }
    }

    spawnPlayer();    
}

void ScenePlay::spawnPlayer()
{
    // Read config file for player
    std::string title {};
    std::ifstream inFile {"config/config.txt"};
    std::string lines {};

    while (std::getline(inFile, lines))
    {
        std::stringstream line {lines};
        line >> title;
        line >> mPlayerConfig.X >> mPlayerConfig.Y >> mPlayerConfig.CX >> mPlayerConfig.SPEED 
             >> mPlayerConfig.MAXSPEED >> mPlayerConfig.JUMP >> mPlayerConfig.GRAVITY 
             >> mPlayerConfig.WEAPON;
    }

    inFile.close();
    

    // For player, 3 animations to get working
    mPlayer = mEntityManager.addEntity(title);
    mPlayer->addComponent<CAnimation>(mGame->assets().getAnimation("Skully"), true);
    Vector midPos { gridToMidPixel(mPlayerConfig.X, mPlayerConfig.Y, mPlayer) };
    mPlayer->addComponent<CTransform>(midPos);
    mPlayer->addComponent<CBoundingBox>(mPlayer->getComponent<CAnimation>().animation.getSize());
    mPlayer->addComponent<CGravity>(mPlayerConfig.GRAVITY);
    mPlayer->addComponent<CState>("Stand");
    mPlayer->addComponent<CInput>();
}

void ScenePlay::spawnBullet(Vector& mousepos)
{
    /*
    Bullet should go in direction player is facing and holding button should not continuously fire.
    Can use CInput.canShoot variable to implement this
    */
    auto entity = mEntityManager.addEntity("Bullet");

    // Spawn bullet at entity firing it
    Vector bPos = mPlayer->getComponent<CTransform>().pos;
    Vector bVel = mousepos - bPos;
    bVel.normalize();

    // Add components
    entity->addComponent<CAnimation>(mGame->assets().getAnimation("Fireball"), true);
    entity->addComponent<CTransform>( bPos, bVel * 10, 0 );
    entity->addComponent<CBoundingBox>(entity->getComponent<CAnimation>().animation.getSize());
    entity->addComponent<CLifespan>(180, mCurrentFrame);
}

void ScenePlay::update()
{
    // Implement pause functionality
    if (!mPaused)
    {
        mEntityManager.update();
        sEnemySpawner();
        sMovement();
        sCollision();
        sLifespan();
        sAnimation();
    }
    sRender();

    mCurrentFrame++;

    if (mCurrentFrame % 100 == 0)
    {
        std::cout << mCurrentFrame << '\n';
        std::cout << "y velocity: " << mPlayer->getComponent<CTransform>().velocity.y << '\n';
    }

    if (mPlayer->getComponent<CInput>().shoot)
    {
        spawnBullet(lastShot);
        mPlayer->getComponent<CInput>().shoot = false;
        coolDown = 25;
    }

    if (coolDown > 0)
    {
        --coolDown; 
        if (coolDown == 0)
        {
            mPlayer->getComponent<CInput>().canShoot = true;
        }
    }
}

void ScenePlay::spawnEnemy()
{
    // Spawn enemy at random position and record when most recent enemy was spawned
    auto entity = mEntityManager.addEntity("Enemy");

    // Spawn enemy with random position within window, random speed between 0ish/5 pixels/s
    float ex { static_cast<float>(rand() % (int)width()) };
    float ey { static_cast<float>(rand() % (int)height()) };

    // Give it transform, shape, and other components
    entity->addComponent<CAnimation>(mGame->assets().getAnimation("Ghost"), true);
    entity->addComponent<CTransform>( Vector { ex, ey } );
    entity->addComponent<CBoundingBox>(entity->getComponent<CAnimation>().animation.getSize());
    entity->addComponent<CLifespan>(250, mCurrentFrame);
}

void ScenePlay::sEnemySpawner()
{
    if (mCurrentFrame % 180 == 0)
    {
        spawnEnemy();
    }
}

void ScenePlay::sMovement()
{
    /*
    Implement player movement/jumping based on CInput component, gravity's effect on player,
    maximum player speed in both horizontal directions. Remember setting scale.x to -1 / 1 will
    make it face left / right
    */
    //Vector playerVelocity { 0, mPlayer->getComponent<CTransform>().velocity.y }; // Maintains gravity
    auto& playerTransform = mPlayer->getComponent<CTransform>();
    /*
    Gravity is a bit complicated. It is acting on all frames but when we land our speed should be
    reset to 0 so we don't have to mash the jump button to make it into the air
    */

    if (mPlayer->getComponent<CInput>().jump)
    {
        playerTransform.velocity.y = -mPlayerConfig.JUMP; 
    }
    else if (mPlayer->getComponent<CInput>().down)
    {
        playerTransform.velocity.y = mPlayerConfig.SPEED;  
    }
    else if (mPlayer->getComponent<CInput>().left)
    {
        //mPlayer->getComponent<CState>().state = "run";  // Update state for animation
        playerTransform.velocity.x = -mPlayerConfig.SPEED;  
    }
    else if (mPlayer->getComponent<CInput>().right)
    {
        playerTransform.velocity.x = mPlayerConfig.SPEED;  
    }

    for (auto& e : mEntityManager.getEntities())
    {
        if (e->hasComponent<CGravity>())
        {
            e->getComponent<CTransform>().velocity.y += e->getComponent<CGravity>().gravity;     
            if (playerTransform.velocity.y >= mPlayerConfig.MAXSPEED)
            {
                playerTransform.velocity.y = mPlayerConfig.MAXSPEED;
            }      
        }
        
        e->getComponent<CTransform>().pos += e->getComponent<CTransform>().velocity;
    }
}

void ScenePlay::sLifespan()
{
    // Check lifespan of entities that have that component and destroy them if they go over
    for (auto& e : mEntityManager.getEntities())
    {
        if (e->hasComponent<CLifespan>())
        {
            e->getComponent<CLifespan>().lifespan--;
            
            if (e->getComponent<CLifespan>().lifespan == 0)
            {
                e->destroy();
            }
        }
    }
}

void ScenePlay::sCollision()
{
    /*

    Implement Physics::GetOverlap() and use it here. Implement bullet/ tile collisions, destroy tiles
    that have Brick animation. Implement player/tile collisions and resolutions, update CState component
    of plyaer to store whether it is currently on the ground or in the air, to be used by Animation
    system. Check to see if player has fallen down a hole, ie y > height(). Don't let player walk off
    left side of map

    If player hits tile from below set velocity to 0 so gravity can act and player doesn't hover. Find a
    way to determine which direction collision came from
    */

    // Start with player checking with all tiles
    Physics p;
    auto& transf = mPlayer->getComponent<CTransform>();
    for (auto& e : mEntityManager.getEntities("Tile"))
    {
        Vector overlap = p.getOverlap(mPlayer, e);
        if (overlap[0] > 0 && overlap[1] > 0)
        {
            // Overlap in both dimensions -> collision. Get direction of collision
            Vector pOverlap = p.getPreviousOverlap(mPlayer, e);
            if (e->getComponent<CTransform>().pos.y > transf.pos.y)
            {   
                transf.pos.y -= transf.velocity.y;
                mPlayer->addComponent<CState>().state = "Stand";
            }
            else if (e->getComponent<CTransform>().pos.x > transf.pos.x)
            {
                transf.pos.x -= transf.velocity.x;
            }
            else if (e->getComponent<CTransform>().pos.y < transf.pos.y)
            {
                transf.pos.y += transf.velocity.y;
            }
            else if (e->getComponent<CTransform>().pos.x < transf.pos.x)
            {
                transf.pos.x += transf.velocity.x;
            }
        }   


    }
    // Now collision with all enemies
    for (auto& e : mEntityManager.getEntities("Enemy"))
    {
        Vector overlap = p.getOverlap(mPlayer, e);
        if (overlap[0] > 0 && overlap[1] > 0)
        {
            Vector pOverlap = p.getPreviousOverlap(mPlayer, e);
            transf.pos -= transf.velocity;
        }
    }
}

void ScenePlay::sDoAction(const Action& action)
{
    if (action.type() == "START")
    {
        if (action.name() == "TOGGLE_TEXTURE")
        {
            mDrawTextures = !mDrawTextures;
        }
        else if (action.name() == "TOGGLE_COLLISION")
        {
            mDrawCollision = !mDrawCollision;
        }
        else if (action.name() == "TOGGLE_GRID")
        {
            mDrawGrid = !mDrawGrid;
        }
        else if (action.name() == "PAUSE")
        {
            setPaused();
        }
        else if (action.name() == "QUIT")
        {
            onEnd();
        }
        else if (action.name() == "JUMP" && mPlayer->getComponent<CInput>().canJump)
        {
            mPlayer->addComponent<CInput>().jump = true;
            mPlayer->addComponent<CState>().state = "Air";
            mPlayer->addComponent<CInput>().canJump = false;
        }
        else if (action.name() == "DOWN")
        {
            mPlayer->getComponent<CInput>().down = true;
        }
        else if (action.name() == "LEFT")
        {
            mPlayer->getComponent<CInput>().left = true;
        }
        else if (action.name() == "RIGHT")
        {   
            mPlayer->getComponent<CInput>().right = true;
        }
        else if (action.name() == "SHOOT" && mPlayer->getComponent<CInput>().canShoot)
        {   
            mPlayer->getComponent<CInput>().shoot = true;
            lastShot = sf::Mouse::getPosition();
            mPlayer->getComponent<CInput>().canShoot = false;
        }
    }
    if (action.type() == "END")
    {

        if (action.name() == "JUMP")
        {
            mPlayer->addComponent<CInput>().jump = false;
        }
        else if (action.name() == "DOWN")
        {
            mPlayer->getComponent<CInput>().down = false;
        }
        else if (action.name() == "LEFT")
        {
            mPlayer->getComponent<CInput>().left = false;
            mPlayer->getComponent<CTransform>().velocity.x = 0;
        }
        else if (action.name() == "RIGHT")
        {   
            mPlayer->getComponent<CInput>().right = false;
            mPlayer->getComponent<CTransform>().velocity.x = 0;
        }
        else if (action.name() == "SHOOT")
        {   
            mPlayer->getComponent<CInput>().shoot = false;
        }
    }
}

void ScenePlay::sAnimation()
{
    // Complete Animation class code first

    /*
    Set animation of player based on CState component. For each entity with an animation, call
    entity->getComponent<CAnimation().animation.update(). If the animation is non repeating and
    has ended, destroy the entity                     
    */

    for (auto& e : mEntityManager.getEntities())
    {
        // This will have to be adjusted for animations that do not repeat
        if (e->tag() == "Player")
        {
            e->getComponent<CAnimation>().animation.update();
        }
    }

    /* if (mPlayer->getComponent<CState>().state == "air")
    {
        mPlayer->addComponent<CAnimation>("air", mGame->assets().getAnimation("air"));
    } */
}

void ScenePlay::onEnd()
{
    // When the scene ends, change back to MENU scene using mGame->changeScene(params)
    mGame->changeScene("MENU", std::make_shared<SceneMenu>(mGame));;
}

/*
RENDER WAS IN THE VIDEO
*/
void ScenePlay::sRender()
{
    // Color the background darker when paused so you know when the game is paused, DOESN'T WORK
    if (!mPaused)
    {
        mGame->window().clear(sf::Color(255, 255, 255));
    }
    else
    {
        mGame->window().clear(sf::Color(50, 50, 250));
    }

    // Set the viewport of the window to be centered on the player if player is far enough right
    auto& pPos = mPlayer->getComponent<CTransform>().pos;
    float windowCenterX { pPos.x };
    float windowCenterY { pPos.y };
    sf::View view = mGame->window().getView();
    view.setCenter(windowCenterX, windowCenterY);
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
                animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
                /* animation.getSprite().setRotation(transform.angle);
                animation.getSprite().setScale(transform.scale.x, transform.scale.y); */
                mGame->window().draw(animation.getSprite());
            }
        }
    }
    
    // Draw all Entity collision bounding boxes with a rectangle shape
    if (mDrawCollision)
    {
        for (auto& e : mEntityManager.getEntities())
        {
            if (e->hasComponent<CBoundingBox>())
            {
                auto& box = e->getComponent<CBoundingBox>();
                auto& transform = e->getComponent<CTransform>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
                rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
                rect.setPosition(transform.pos.x, transform.pos.y);
                rect.setFillColor(sf::Color(0, 0, 0, 0));
                rect.setOutlineColor(sf::Color(255, 255, 255, 255));
                rect.setOutlineThickness(1);
                mGame->window().draw(rect);
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

    mGame->window().display();
}

