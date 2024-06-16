#include <iostream>
//#include "game.h"
#include "vector.h"
#include "game_engine.h"
#include <random>

/*
// Special case for if user takes screenshot
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::X)
            {
                // Create texture from current window and copy to PNG
                std::cout << "Saving screenshot...\n";
                sf::Texture texture;
                texture.create(mWindow.getSize().x, mWindow.getSize().y);
                texture.update(mWindow);
                if (texture.copyToImage().saveToFile("test.png"))
                {
                    std::cout << "Screenshot saved to " << "test.png" << '\n';
                }
            }
        }
*/

/*
Eventually I will need to get OpenGL involved. I want to note now that the
class reference ContextSettings is a structure that defines the settings of the OpenGL
context attached to the window. SFML is programmed over OpenGL, so they are quite 
compatible. We can use the setActive member function from RenderWindow to select it as
the rendering target

EXTRA PROJECTS:

Config files - I have the config file for player working. However, there should be more/more information
in that config file, which will take a while to code. I need: system, small/bigenemy, bullet, and special.
Either make multiple config files or put it all in one. Probably put it all in one

Implement power up - temporarily increase player size when player collides with it. Should not have
collision with enemies, NEW COMPONENT

Error handling - A constant process. First handle FileException. Other possibilities are things like
OverflowException, for if the container of active entity pointers goes above a certain value; 
OutOfBoundsException, for if an entity is trying to render but is outside the window's border;
etc.

Ray casting - fire a line segment from an enemy, if I intersect with that line segment, they can see me.
Line of sight. Ray tracing asks "where did the line bounce to?". 2D ray casting can also be used for 
lighting, shaders, etc All about line intersection. For lines r = b - a, s = d - c, scale r by t and 
s by u. Find t, u such that r and s meet at their tip, which is satisfied by b = d, or 
a + t * r = c + u * s. The lines interesect if both t, u [0, 1], otherwise the line segments are not 
between their respective points; if they were outside that interval, that would imply the collision happens
"outside the line segment." We can use 2D cross product a x b = a.x * b.y - a.y * b.x. Clearly a x a = 0.
Also (a + b) x c = a x c + b x c. Ultimately we reach the following function:

struct Intersect { bool result; Vector pos; }; If and when they intersect
Intersect LineIntersect(Vector a, b, c, d) 4 points
{
    Vector r = b - a;
    Vector s = d - c;
    float rxs = rxs;
    Vector cma = c - a;
    float t = (cma x s) / rxs;
    float u = (cma x r) / rxs;
    if (t >= 0 && t <= 1 && u >= 0 && u <= 1);
        return (true, Vector (ax + t * rx, ay + t * ry));
}
*/
/*
With some edits the following code could be reallly useful for tracking memory usage. Have a tough time making
it go above 2kB.
From my understanding, memory usage for game instance with N entities is approximately equal to:
N * (sizeof(Entity) + 2 * sizeof(std::shared_ptr<Entity>)) + sizeof(overhead)
where overhead is memory used by allocated variables, constants, etc. 
Worth noting that the game class itself is 1.5kB big on its own. So I'm likely using significantly more memory
than I think I am

if (mCurrentFrame % 300 == 0)
    {
        int mem { 0 };
        for (auto& e : mEntities.getEntities())
        {
            mem += sizeof(*e);
        }
        std::cout << sizeof(std::shared_ptr<Entity>) << '\n';
        std::cout << "Memory used by entities at frame " << mCurrentFrame << ": " << mem << " bytes\n";
    }
*/

/*
TODO:

- Architecture: GameEngine, Scene (Abstract Base), Scene_Play: Scene, Scene_Menu: Scene, EntityManager, Entity,
Assets, Animation, Vector, Action, Physics, Component (Interface), CTransform, CBoundingBox

*/

// DETAILS
/*
- Entities will be rendured with various textures, fonts, and animations, generalized as Assets, which will be loaded
only once at the beginning of the program and stored in the Assets class. They will be defined in Assets.txt as
follows:

    Asset Specification:
    Asset Format
        Asset Data  (no spaces if that makes it too hard)

- All entity positions denote the center of their rectangular sprite, aligned with the center of their bounding box,
set via sprite.setOrigin in the Animation class constructor

- The player entity should have 3 different animations: stand, run, air. Must determine current state then assign
correct animation

- Player can move left A right D jump W shoot SPACE at any time, including in mid air. Can only jump if standing on
a tile. If jump is held, max height, if you let go early, fall down immediately. Holding jump should not make player 
continuously jump, one jump per press

- Pressing left or right makes player face that direction until they press the other, set scale.x = -1. Bullets
shot travel in direction player is facing

- Player collides with Tile entities and cannot move through them from any direction. Player does not collide with
docoration Dec entities

- If player falls below bottom of screen they respawn at start

- Player should have CGravity component which constantly accelerates down until player collides with tile

- Player has maximum speed that is the same in both left and right

- Player has CBoundingBox and Sprite centered on player's position

- Animations are stored as multiple frames in one texture. Animation class handles frame advancement based on
animation speed, animations are progressed by Animation::update(). Animation::hasEnded() should return true if 
animation has finished its last frame, false otherwise

- Animations can be either repeating or non repeating, any entity with non repeating animation should be destroyed
once hasEnded() returns true

- Decorations do not interact with other entities in any way

- Tiles are entities that define level geometry and do interact with player. Tile CBoundingBox size is equal to
size of animation tile->getComponent<CAnimation>().animation.getSize()

- Current animation displayed for tile can be retreived with tile->getComponent<CAnimation>().animation.getName()

- Differently animated tiles have different behavior

- If a tile is destroyed then its bounding box must also be destroyed. Implement toggle for displaying textures and

- I DO NOT HAVE AN EXPLICIT GUIDE FOR RENDERING that'll be fun

- P should pause the game, T toggles drawing textures, C toggles drawing bounding boxes, G toggles grid, ESC goes
back to main menu during play scene and quits from menu

- Overall goal is to create one level specified by a level file that has a list of entity specifications, one
entity per line, with a single line which specifies the properties of the player in that level

- Level uses grid coordinates with the grid defined by the size of the textures
*/

// PROGRESS
/*
Start by editing current geometry wars game to render a row of sprites along the bottom of the window that no
entities with collision can overlap with or travel through

WHEW THAT WAS A LOT
*/


int main()
{
    srand(std::random_device{}());

    //Game game { "config/config.txt" };
    GameEngine game { "config/assets.txt" };
    
    game.run();

    return 0;
}