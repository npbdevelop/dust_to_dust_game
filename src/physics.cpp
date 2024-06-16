#include "physics.h"

/*
Return overlap rectangle size of bounding boxes of a and b
First make sure both a and b have transforms and bounding boxes, then compute
and return the overlap vector. If either entity has no bounding box return 0, 0
*/
Vector Physics::getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{   
    if (a->hasComponent<CTransform>() && a->hasComponent<CBoundingBox>() &&
        b->hasComponent<CTransform>() && b->hasComponent<CBoundingBox>())
    {
        // Get a and b positions, both of which are measured from their animation's center
        auto& aPos = a->getComponent<CTransform>().pos; 
        auto& bPos = b->getComponent<CTransform>().pos;

        auto halfAW = a->getComponent<CBoundingBox>().halfSize;
        auto halfBW = b->getComponent<CBoundingBox>().halfSize;

        // Distance between midpoints
        auto delta = Vector { std::abs(bPos.x - aPos.x), std::abs(bPos.y - aPos.y) };

        auto overlap = halfBW + halfAW - delta;
        return overlap;
    }
    else
    {
        return { 0, 0 };
    }
}

Vector Physics::getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    /*
    Same as above but use previous positions
    */
    if (a->hasComponent<CTransform>() && a->hasComponent<CBoundingBox>() &&
        b->hasComponent<CTransform>() && b->hasComponent<CBoundingBox>())
    {
        // Get a and b positions, both of which are measured from their animation's center
        auto& aPos = a->getComponent<CTransform>().prevPos; 
        auto& bPos = b->getComponent<CTransform>().prevPos;

        auto halfAW = a->getComponent<CBoundingBox>().halfSize;
        auto halfBW = b->getComponent<CBoundingBox>().halfSize;

        // Distance between midpoints
        auto delta = Vector { std::abs(bPos.x - aPos.x), std::abs(bPos.y - aPos.y) };

        auto overlap = halfBW + halfAW - delta;
        return overlap;
    }
    else
    {
        return { 0, 0 };
    }
}