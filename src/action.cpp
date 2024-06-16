#include "action.h"

Action::Action(const std::string& name, const std::string& type)
    : mName { name }, mType { type }
{

}

const std::string& Action::name() const
{
    return mName;
}

const std::string& Action::type() const
{
    return mType;
}