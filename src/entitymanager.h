#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <array>
#include <vector>
#include <memory>
#include <algorithm>
#include <map>
#include <iostream>
#include "entity.h"

/*
The vector of shared pointers gives us a way to always keep track of entities, and the map
allows us to find them quickly. We don't store the raw objects because when something is added or removed,
the whole vector shifts and copies itself, expensive
*/
typedef std::vector<std::shared_ptr<Entity>> EntityVector;
typedef std::map<std::string, EntityVector>  EntityMap; // Allows us to access entire categories easily
class EntityManager
{
private:
    EntityVector mEntities {};
    EntityVector mToAdd {};    // Helps delay updates to vectors to prevent iterator invalidation
    EntityMap    mEntityMap {};
    size_t       mTotalEntities {};  // Running count of *all time*, preserves uniqueness

    // Saw a repeat void removeDeadEntities(EntityVector& v) in tutorial that was private

public:
    EntityManager();

    void init();
    void update();  // Runs at the beginning of each frame, adding entities from last frame
    std::shared_ptr<Entity> addEntity(const std::string& tag);
    void removeDeadEntities(EntityVector& entitiesV);

    const EntityVector& getEntities();
    const EntityVector& getEntities(const std::string& tag);
};

#endif