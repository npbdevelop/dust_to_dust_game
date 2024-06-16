#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "vector.h"
#include "animation.h"
#include <string>
#include <SFML/Graphics.hpp>

/*
All components inherit this abstract class which allows us to more easily enable and disable
them
*/
class Component
{
public:
    bool has { false };
};

/*
Component containing physical transformation data such as position, movement, scale, and relative
angle. It gives us consistent access to the information necessary to render changes to an object's
physical state
*/
class CTransform : public Component
{
public:

    CTransform() = default;
    CTransform(const Vector& p)
        : CTransform(p, { 0, 0 }, 0)
    {}
    CTransform(const Vector& p, const Vector& v, float a);

    Vector pos;
    Vector prevPos;    // Helps with collision, update each frame before updating pos
    Vector scale;
    Vector velocity;
    float angle;
};

/*
AABB overlap from centers of bounding boxes: 
delta = [abs(x1 - x2), abs(y1 - y2)] distance vector between centers of rectangles
ox = (w1 / 2) + (w2 / 2) - delta.x
oy = (h1 / 2) + (h2 / 2) - delta.y
overlap = [ox, oy]  ->  if both are positive, rectangles overlap

RESOLVING COLLISIONS: if, for example, an entity collides with a wall, they should be pushed back by the amount
they are overlapping. So [ox, oy] is very useful

Other exaamples: diagonal overlap like stairs, push up by oy, hit ledge at waist height, push back by ox

Could detect which direction collision came frome using prevPos to calculate prevOverlap and push back in
the direction that oPrev was positive; ex, a collision came from above if on previous frame 
ox > o and prevY > yNow

What about if ox, oy < 0 on previous frame? What if multiple collisions at once?

For multiple, if we use overlap direction we should be ok
*/

/*
UNDER CONSTRUCTION seems like this essentially wraps around the sf::FloatRect class returned by 
sf::getLocal/GlobalBounds, but it is not yet implemented. We will be using AABB overlap for collision
detection, details are above
*/
class CBoundingBox : public Component
{
public:

    CBoundingBox() = default;
    CBoundingBox(const Vector& s);

    Vector size;
    Vector halfSize;    // Have to compute this so often it's worth storing for each entity
};

/*
Additive factor to velocity vectors for all entities at all times, causes falling in mid air. Don't
want this to exceed initial velocity up, should actually be quite small otherwise jumping will look
glitchy since it is added every frame. Note it is positive because the origin is at the top left
*/
class CGravity : public Component
{
public:

    CGravity() = default;
    CGravity(float g);

    float gravity;
};

/*
Stores a string as a state to be read by sAnimation to determine which animation to play for a given
entity on a given frame
*/
class CState : public Component
{
public:

    CState()
        : state { "Air" }
    {}
    CState(const std::string & s);

    std::string state;
};

/*
Responsible for storing an entity's animation textures. Also flags whether the animation plays on loop,
or just once then gets destroyed
*/
class CAnimation : public Component
{
public:

    CAnimation() = default;
    CAnimation(const Animation& an, bool r);

    Animation animation;
    bool repeat;
};

/*
Stores the total lifespan in frames of an entity and initializes its remaining lifespan at that total
value. This value is decremented every frame for entities possessing this component
*/
class CLifespan : public Component
{
public:

    CLifespan() = default;
    CLifespan(int duration, int frame);

    int lifespan;    
    int frameCreated;
};

class CInput : public Component
{
public:

    CInput()
    : jump { false }, left { false }, right{ false }, down { false }
    , shoot { false }, canShoot { true }, pause { false }
    , canJump { true }
    {}

    bool jump;
    bool left;
    bool right;
    bool down;
    bool shoot;
    bool canShoot;
    bool pause;
    bool canJump;
};

#endif