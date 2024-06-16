#ifndef PHYSICS_H
#define PHYSICS

#include <map>
#include "vector.h"
#include "entitymanager.h"

class Physics
{
private:

public:

    Physics() = default;

    Vector getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
    Vector getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
    Vector acceleration(std::shared_ptr<Entity> v);
};


#endif