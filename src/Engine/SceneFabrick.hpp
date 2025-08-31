#pragma once

#include <memory>

#include "BaseScene.hpp"
#include "Types.hpp"

#include "../Scenes/MenuScene.hpp"

namespace engine
{

    using SceneUPtr = std::unique_ptr<Scene>;

    class SceneGenerator
    {
    public:
        SceneGenerator(sdlwrapper::RenderWindow& render, sdlwrapper::EventBus& bus, rmluiwrapper::Context& context) :
            render_{render}, bus_{bus}, context_{context}
        {}

        SceneUPtr generateScene(const IDType id)
        {
            if(id == 1) return std::make_unique<MenuScene>(render_, bus_, context_);
            // if(id == 2) return std::make_unique<GameScene>(render_, bus_, fonts_);
            // if(id == 3) return std::make_unique<TestScene>(render_, bus_, fonts_);
            return nullptr;
        }

    private:

        sdlwrapper::RenderWindow& render_;
        sdlwrapper::EventBus& bus_;
        rmluiwrapper::Context& context_;

    };

} // namespace engine
