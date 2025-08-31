#pragma once

#include "../SDLWrapper/SDLWrapper.hpp"
#include "Types.hpp"
#include "IterateResult.hpp"

#include "../RmlUi/RmlUi.hpp"

namespace engine
{
    
    class Scene : public sdlwrapper::Updateable
    {
    public:
        Scene(sdlwrapper::RenderWindow& render, sdlwrapper::EventBus& bus, rmluiwrapper::Context& context) : render_{render}, bus_{bus}, context_{context}
        {}
        virtual ~Scene() = default;

        virtual void drawScene() const {}

        virtual void updateEvents(const SDL_Event* event) override {}

        virtual IterateResult update()
        {
            return updateRes_;
        }

    protected:

        sdlwrapper::RenderWindow& render_;
        sdlwrapper::EventBus& bus_;
        rmluiwrapper::Context& context_;

        IterateResult updateRes_ = IterateResult::next();
        
    };

} // namespace engine
