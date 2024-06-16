#ifndef ASSETS_H
#define ASSETS_H

#include <SFML/Graphics.hpp>
#include <string>
#include <string_view>
#include <fstream>
#include <map>
#include <format>
#include <iostream>
#include <sstream>
#include <array>
#include "animation.h"
#include "file_exception.h"

typedef std::map<std::string, sf::Font> FontMap;
typedef std::map<std::string, Animation> AnimationMap;
class Assets
{
private:

    FontMap mFontMap;
    AnimationMap mAnimationMap;

public:

    Assets() = default;

    void loadFromFile(const std::string& path);
    const sf::Font& getFont(const std::string& font);
    Animation& getAnimation(const std::string& animation);
};


#endif