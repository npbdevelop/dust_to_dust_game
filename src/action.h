// Responsible for converting arbitrary input to specific action

/*
Action objects have a name and a type, and type is analogous to keypressed vs keyreleased; start/end. In SFML
all keyboard keys have corresponding int values through an enum
*/

#ifndef ACTION_H
#define ACTION_H

#include <string>

class Action
{
private:

    std::string mName = "NONE"; //e.g. { "Right", "Start" } and { "Right", "End" }
    std::string mType = "NONE";

public:

    Action() = default;
    Action(const std::string& name, const std::string& type);

    const std::string& name() const;
    const std::string& type() const;
    std::string toString() const;
};

#endif