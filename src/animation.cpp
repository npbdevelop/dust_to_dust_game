#include "animation.h"

Animation::Animation(const std::string& name, const sf::Texture& t)
    : Animation { name, t, 1, 0 }
{}

Animation::Animation(const std::string& name, const sf::Texture& t, int frameCount, int speed)
    : mName { name }, mSprite { t }, mFrameCount { frameCount }
    , mCurrentFrame { 0 }, mSpeed { speed }
{
    mSize = Vector { (float)t.getSize().x / frameCount, (float)t.getSize().y };
    mSprite.setOrigin(mSize.x / 2.f, mSize.y / 2.f);
    mSprite.setTextureRect(sf::IntRect(std::floor(mCurrentFrame) * mSize.x,
        0, mSize.x, mSize.y));
}

/*
Updates animation to show next frame depending on speed, animation loops when it reaches
the end
*/
void Animation::update()
{
    // Add speed variable to current frame
    ++mCurrentFrame;
    if ((mFrameCount * mSpeed) % mCurrentFrame == 0 && mFrameCount > 1)
    {
        mSprite.setTextureRect(sf::IntRect(std::floor(mCurrentFrame / mSpeed) * mSize.x, 0, mSize.x, mSize.y ));
    }

    if (std::floor(mCurrentFrame / mSpeed) == mFrameCount - 1)
    {
        mCurrentFrame = 0;
    }
}

const Vector& Animation::getSize() const
{
    return mSize;
}

const std::string& Animation::getName() const
{
    return mName;
}

bool Animation::hasEnded() const
{
    // Detect when animation has ended (last frame was played) and return true
    return (mCurrentFrame == mFrameCount ? true : false);
}

sf::Sprite& Animation::getSprite()
{
    return mSprite;
}