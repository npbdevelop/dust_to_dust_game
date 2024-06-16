#include "entitymanager.h"

EntityManager::EntityManager()
    : mEntityMap { { "Dec", {} }, { "Tile", {} }, { "Player", {} }, { "Enemy", {} }, { "Bullet", {} } }
    , mTotalEntities { 0 }
{
    init();
}

void EntityManager::init()
{
    // IDK what this is for
}

/*
Called on its own at the beginning of each frame. On a delay to avoid iterator invalidation;
pushes new entities onto mEntities and mEntityMap (according tag for the latter), clears the
queue for the coming frame, and removes all dead entities.
*/
void EntityManager::update()
{
    for (auto& e : mToAdd)
    {
        mEntities.push_back(e);
        mEntityMap[e->tag()].push_back(e);
    }

    mToAdd.clear(); // Don't want to accidentally make copies

    // Remove dead entities from vector of all entities, and map
    removeDeadEntities(mEntities);

    for (auto& [tag, eV] : mEntityMap)
    {
        removeDeadEntities(eV);
    }
} 

/*
Adds a shared pointer for each new entity and keeps track of how many we have. First adds entity to buffer
mToAdd to avoid iterator invalidation, then update() is called at the beginning of each frame to update
the full entity vector and clear the buffer. 
*/
std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{   
    // Give each entity an increasing integer id, increment as we create more
    auto e = std::shared_ptr<Entity>(new Entity(mTotalEntities++, tag));

    mToAdd.push_back(e);    // Middleman container to pull from when it's safe 
    
    return e;
}

/*
Removing an entity actually requires that we remove it from the vector itself. The danger
is iterator invalidation, but C++ gives us std::remove_if, allowing us to remove all entities
for whome the value of mActive is false
*/
void EntityManager::removeDeadEntities(EntityVector& entities)
{
    entities.erase(std::remove_if(entities.begin(), entities.end(),
        [](std::shared_ptr<Entity> ePtr) 
        {
            return !(ePtr->isActive());
        }), entities.end()); 
}

const EntityVector& EntityManager::getEntities()
{
    return mEntities;
}

const EntityVector& EntityManager::getEntities(const std::string& tag)
{
    return mEntityMap[tag];
} 

