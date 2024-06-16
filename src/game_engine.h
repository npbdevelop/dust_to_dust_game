/*
Stores top level game data: assets, window, scenes

-map<string scene>
-sfrenderwindow window
-assets: assets
-string currentscene
-bool running
-void init
-scene* currentscene
+void run()
+void update()
+void quit()
+void changescene(scene)
+assets& getassets()
+window& window()
+void suserinput()

Loads all assets at once at the beginning. Modern games load assets between scenes, ie loading screens. Actual
loading of raw input happens here. Ideally game engine handles raw inputs, creates events, passes them off. 
run() contains main loop, and the GameEngine pointer will be passed to Scenes in constructor for access to assets,
window, and scenes

Together, GameEngine and Scene allow for scene switching. GameEngine stores a map from strings to shared_ptr<Scene>
so we can just call map[currentScene] (as long as we update currentScene) to get the shared pointer and then call
changeScene(string, scene) to a new or previously stored scene

One big challenge is asset management. "Load once, use often" don't load the asset from disk every frame you're 
rendering it, that's stupid. So we will make an Assets class that can load an Asset (Texture, Sound) with a name
(MegaMan, DeathSound) at a file path. When we load it the Assets object will then store it and we can reference
it when we need it via mAssets->getTexture("MegaMan"). The Asset class will use a map<string, asset type> to
implement this

Assets class is contained within the GameEngine class and initialized inside GameEngine init()

-map<string, sf::shape> mTextures
-map<string, animation> manimations
-map<string, sf::sound> msounds
-map<string, sf::font> mfonts
+void addtexture(name, path)
+void addanimation(name, path)
+void addsound(name, path)
+void addfont(name, path)
+sf::texture& gettexture(name)
+animation& getanimation(name)
+sf::sound& getsound(name)
+sf::font& getfont(name)

sfml works much like sdl for texture loading. We can declare sf::Texture texture and texture.loadFromFile(path). We
can also call texture.update(window) to update the contents of a texture based on the contents of a window, and
even write those changes to a file. If a texture is not the same size as its entity's shape, rather than squishing
or stretching we can specify a sub rectangle of the texture to draw. The entire texture is loaded, but only 
part is drawn. With all this said:
            A sprite is the specified sub rectangle of a texture that has been rendered to an entity
We create sprites with textures; sf::Sprite sprite; sprite.setTexture(texture); window.draw(sprite); Note that 
sprites are actually pointers to textures, so we have to be careful about the lifetime of the texture object
so that the sprite doesn't unexpectedly point to null. This is why we want the assets class to have the same lifetime
as the game engine

Texture sheets are large images containing a bunch of different textures, and we pre define the subrectangles 
corresponding to each texture, then call them when we need them
*/

#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "scene.h"
#include "file_exception.h"
#include "assets.h"
#include "sceneplay.h"
#include "scenemenu.h"

typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine
{
protected:

    sf::RenderWindow mWindow;
    Assets mAssets;
    std::string mCurrentScene;
    SceneMap mSceneMap;
    size_t mSimulationSpeed;
    bool mRunning { true };

    void init(const std::string& path);
    void update();

    void sUserInput();

    std::shared_ptr<Scene> currentScene();

public:

    GameEngine(const std::string& path);

    void changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene,
        bool endCurrentScene = false);

    void quit();
    void run();

    sf::RenderWindow& window();
    Assets& assets();
    bool isRunning();
};

#endif