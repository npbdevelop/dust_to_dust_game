#ifndef ANIMATION_H
#define ANIMATION_H

#include "vector.h"

#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>

class Animation
{
private:

    sf::Sprite mSprite;
    int mFrameCount { 1 };   // Total number of frames of animation
    int mCurrentFrame { 0 }; // Current frame relative to current animation
    int mSpeed { 0 };    // How many frames each sprite is rendered for animation
    Vector mSize { 1, 1 };  // Size of animation frame
    std::string mName;

public:

    Animation() = default;
    Animation(const std::string& name, const sf::Texture& t);
    Animation(const std::string& name, const sf::Texture& t, int frameCount, int speed);

    void update();
    bool hasEnded() const;
    const std::string& getName() const;
    const Vector& getSize() const;
    sf::Sprite& getSprite();
};


#endif