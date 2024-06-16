/*
Notes on abstract base classes:
Relates to polymorphism/virtual functions. virtual override tells compiler to look for the most derived
definition of a function when the function is called from a pointer. So a base pointer to a derived class
calling a function with polymorphism will return the derived result if the base function(s) are overriden
as virtual. Note that once a base function is made virtual, all matching overrides in its derived classes
are implicitly virtual. When derived class objects are created, the base class is constructed first. All 
of this together implies we should never call virtual functions from constructors or destructors

Useful tools for these things are the identifiers override and final, especially override. Identifying
a member function with override will tell the compiler that it is overriding a virtual function with the same
name. If that is incorrect, the compiler will throw an error; for example, if the parameter types don't
match, or if the cv identifiers don't match. Always use override when override behavior is desired. Final
identifies a function as the most derived allowed version, so if anything attempts to inherit and override
it, the compiler will give an error. The only time a derived function can override a base function with
a different return type is when the return value is a pointer, where the base returns a base pointer that is
base to the derived function return's derived pointer. We call these covariant return types, and if the 
derived function is marked override, the derived pointer will be returned instead of the base pointer,
even though the functions have different return types

Whenever dealing with inheritance, ALL (non final) explicit destructors should be marked virtual, to ensure the derived
object is fully deleted. Rule of thumb: a base class destructor should be either public and virtual, or
protected and non virtual. If we REALLY need to get around the virtual identifier we can use scope resolution
::. Best practice: 
    If inheritance is intended, mark destructor virtual. If it is not intended, mark it final
All other circumstances fall in a gray area that requires thought and care

Pure virtual functions are virtual functions in a base class with empty bodies. They are placeholders meant
to be redefined by derived classes; rather than defining a body for them, we write virtual type f() = 0;.
Any class with one or more pure virtual functions becomes an abstract base class, and an abstract base class
can not be instantiated. In addition, any derived classes which do not define a body for their abstract base's
pure virtual function(s) will become abstract base classes. However, if we have an abstract base pointer to
a derived class, we can call a pure virtual function from it, and the call will resolve to the derived class's
virtual function and give the correct return value. Pure virtual functions can be given external definitions,
but not inline; ie separate from the declaration, or they must be forward declared

Interface classes are classes with no member variables and only pure virtual functions. Combined with the 
pointer behavior discussed earlier, this allows extreme flexibility when passing derived class objects by
reference: for example, an error log interface class could have several child classes, and if a function passes
an interface class object by reference, then any derived class object we pass through a base pointer will
be accepted and resolve to the correct derived implementation

Base classes can be marked virtual before the access specifier in the inheritance statement to make their peers
in the virtual table (same level of inheritance) not make multiple copies of the base class. This works by 
forcing the derived class's constructor that was called to skip the intermediate classes and construct the
base class itself, the only time this is possible

Object slicing occurs when a derived object is assigned to a base object, rather than a derived object being pointed
to or referenced by a base pointer or reference. The object is "sliced" in that only its base part is copied
by the copy constructor

The operator dynamic_cast will downcast a base pointer to a derived object to a derived pointer, which is not 
possible by implicit type deduction. However, if the dynamic_cast fails, likely due to the casted pointer not 
actually pointing to a derived class object, the compiler will not throw an error, and a nullpointer will be 
created, causing dereferencing issues and possible undefined behavior
*/

/*
Scene abstract base class stores common scene data like entities, frame count, and actions

-GameEngine* game
-EntityManager entities
-int currentframe
-map<int, string> actionmap
-bool paused
-bool hasended
+void update() = 0
+void sdoaction(action) = 0
+void srender() = 0

+void simulate(int)
+void doaction(action)
+void registeraction(action)

Scene specific functionality will be carried out in derived classes. simulate() calls derived scene's update()
a given number of times

Scene derived class stores scene specific data like level, player pointer, config

-string levelpath
-entity* player
-playerconfig playerconfig
+void init
+void update()

This is the level at which we will implement actual systems

-void sAnimation()
-void sMovement()
-void sEnemySpawner()
-void sCollision()
-void sRender()
-void sDoAction(action)
-void sDebug

Not all scenes will look the same on the inside, some scene derived classes may require very different functionality.
But update, sRender, and sDoaction are needed for EVERY scene class. Note that the abstract class has exactly these
as its pure virtual functions
*/

#ifndef SCENE_H
#define SCENE_H

#include "action.h"
#include "entitymanager.h"

#include <memory>

class GameEngine;

typedef std::map<int, std::string> ActionMap;

class Scene
{
protected:

    GameEngine* mGame;
    EntityManager mEntityManager;
    ActionMap mActionMap;
    bool mPaused { false };
    bool mHasEnded { false };
    int mCurrentFrame { 0 };

    Vector mGridSize {128, 128}; // Width and height of each grid box

    virtual void onEnd() = 0;
    void setPaused();

    sf::Text mGridText;

public:

    Scene() = default;
    Scene(GameEngine* gameEngine);

    virtual ~Scene() = default;
    
    virtual void update() = 0;
    virtual void sDoAction(const Action& action) = 0;
    virtual void sRender() = 0;

    virtual void doAction(Action action); // Receives action from GameEngine, passes it to subclass
    void simulate(const int frames);
    void registerAction(int inputKey, const std::string& actionName);
    Vector gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);

    int currentFrame() const;

    bool hasEnded() const;
    const ActionMap& getActionMap() const;
    void drawLine(const Vector& p1, const Vector& p2);

    float height();
    float width();
};

#endif