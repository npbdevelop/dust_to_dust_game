/*
The gist of how entities will be programmed is that everything in the game is an entity, and we can
turn on specific components to give specific entities specific abilities and behaviors. This program
will contain some of the basic components
*/

#include "components.h"

CTransform::CTransform(const Vector& p, const Vector& v, float a)
    : pos { p }, prevPos{ 0, 0 }, velocity { v }, angle { a }
{}

CGravity::CGravity(float g)
    : gravity { g }
{}

CAnimation::CAnimation(const Animation& an, bool r)
    : animation { an }, repeat { r }
{}

CBoundingBox::CBoundingBox(const Vector& s)
    : size { s }, halfSize { s / 2 }
{}

CLifespan::CLifespan(int duration, int frame)
    : lifespan { duration }, frameCreated { frame }
{}

CState::CState(const std::string & s)
    : state { s }
{}