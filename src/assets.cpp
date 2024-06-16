#include "assets.h"

void Assets::loadFromFile(const std::string& path)
{
    /*
    Load all assets for the game only one time, at the beginning of the program, here. All assets
    are defined in assets.txt. There are 3 kinds  of assets: textures, animations, and fonts.
    They have slightly different line types, but no spaces to make it easy for now. eg Name Path 
    actually appears as namepath. The format is as follows:

    Texture Asset Specification - Texture N P
    Texture Name Path - string string

    Animation Asset Specification - Animation N T F S
    Animation Name Texturename Framecount animSpeed - Animation string string int int

    Font Asset Specification - Font N P
    Font Name Path

    Note for animations the T refers to an existing texture, so the Animation class object does actually
    associate the two. Animations are spritesheets which are both textures and animations
    */

    // Open file and verify it opened

    std::ifstream inFile { path };
    if (!inFile)
    {
        throw FileException(path, "fopen");
    }

    std::string name {};
    std::string thisPath {};
    std::array<int, 2> thisFrames {};

    int lineCount { 1 };
    std::string lines {};
    while (std::getline(inFile, lines)) // Remember to use getline since separating words with spaces makes this easier
    {
        // Start string stream and read type to ensure type is valid

        std::stringstream line {lines};
        line >> name;
    
        if (name == "Texture")
        {
            // Record name of and path to asset
            line >> name >> thisPath;

            // Load asset
            sf::Texture* thisTexture = new sf::Texture;
            if(!thisTexture->loadFromFile(thisPath))
            {
                throw FileException(thisPath, "load");
            }

            // Store name of asset and asset itself in corresponding map
            Animation* thisAnimation = new Animation { name, *thisTexture };
            mAnimationMap.insert({ name, *thisAnimation });
            ++lineCount;
            continue;
        }
        else if (name == "Animation")
        {
            // For animations record number of frames and frame speed
            line >> name >> thisPath >> thisFrames[0] >> thisFrames[1];

            sf::Texture* thisTexture = new sf::Texture;
            if(!thisTexture->loadFromFile(thisPath))
            {
                throw FileException(thisPath, "load");
            }

            Animation* thisAnimation = new Animation { name, *thisTexture, thisFrames[0], thisFrames[1] };
            mAnimationMap.insert({ name, *thisAnimation });
            ++lineCount;
            continue;
        }
        else if (name == "Font")
        {
            line >> name >> thisPath;

            sf::Font* thisFont = new sf::Font;
            if(!thisFont->loadFromFile(thisPath))
            {
                throw FileException(thisPath, "load");
            }

            mFontMap.insert({ name, *thisFont });
            ++lineCount;
            continue;
        }   
        else 
        {
            // Throw exception if a line was not valid
            throw FileException(path, "parse", lines, lineCount);
        }

        /*
        The level specifications will be held in a level.txt files that is formatted with categories
        like Tile, Decoration, Enemy, etc. and that file will be formatted as Type(Tile, Dec, etc) 
        Name(Brick, Ground, etc) Gx Gy. Gx and Gy are grid coordinates, so 0, 0 is the bottom left grid
        space of a level that can fit, in my current case, a 128x128 texture
        */
    } 
}
    
const sf::Font& Assets::getFont(const std::string& font)
{
    return mFontMap[font];
}

Animation& Assets::getAnimation(const std::string& animation)
{
    return mAnimationMap[animation];
}
