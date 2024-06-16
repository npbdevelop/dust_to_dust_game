#include "entity.h"

Entity::Entity(const size_t& id, const std::string& tag)
    : mID { id }, mTag { tag }
{}

void Entity::destroy()
{
    mActive = false;
}

bool Entity::isActive() const
{
    return mActive;
}

const std::string& Entity::tag() const
{
    return mTag;
}

const size_t Entity::id() const 
{
    return mID;
}