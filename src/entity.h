#ifndef ENTITY_H
#define ENTITY_H

#include "components.h"
#include <memory>
#include <string>
#include <tuple>

class EntityManager;

/*
Tuple elements are stored contiguously in memory, similar to arrays. This is much better for memory
management and caching
*/
typedef std::tuple<
    CTransform,
    CLifespan,
    CInput,
    CBoundingBox,
    CAnimation,
    CGravity,
    CState
> ComponentTuple;

class Entity
{
private:

    friend class EntityManager;

    bool mActive { true };
    size_t mID { 0 };
    std::string mTag { "default" };
    ComponentTuple mComponents;

    // Private constructor; no way to create entities except through friend class EntityManager
    Entity(const size_t& id, const std::string& tag);

public:

    // Private member access functions
    bool isActive() const;
    const std::string& tag() const;
    const size_t id() const;
    void destroy();

    template <typename T>
    bool hasComponent()
    {
        return getComponent<T>().has;
    }

    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs)
    {
        auto& component = getComponent<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.has = true;
        return component;
    }

    template<typename T>
    T& getComponent()
    {
        return std::get<T>(mComponents);
    }


    template<typename T>
    void removeComponent()
    {
        getComponent<T>() = T();
    }
};

#endif