#pragma once

#include <string>
#include <map>

#define FONTS_FILE "xml/fonts.xml"
#define MAIN_SCENE_FILE "xml/MainMenu.xml"
#define GAME_SCENE_FILE "xml/GameScene.xml"
#define TEST_SCENE_FILE "xml/TestSceneInit.xml"

namespace engine
{

    using IDType = unsigned short;
    using RawJson = std::map<std::string, std::string>;

}