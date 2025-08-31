#pragma once

#include <vector>

#include "../SDLWrapper/SDLWrapper.hpp"
#include "../RmlUi/RmlUi.hpp"

#include "BaseScene.hpp"
#include "SceneFabrick.hpp"
#include "IterateResult.hpp"

namespace engine
{

    class Engine
    {
    public:
        Engine() : generator_{render_, bus_, context_}
        {
        }
        ~Engine() = default;

        SDL_AppResult start(const IDType startSceneId, const char *windowName)
        {
            if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
                return SDL_APP_FAILURE;
                
            if (!render_.openWindow(sdlwrapper::VideoMode::defaultMode(), windowName))
                return SDL_APP_FAILURE;

            if (!context_.initRmlUi(render_.getNativeRender(), render_.getNativeWindow(), "fonts/fonts.txt"))
                return SDL_APP_FAILURE;
                
            if(!(curentScene_ = generator_.generateScene(startSceneId)))
                return SDL_APP_FAILURE;

            bus_.subscribe(context_);

            return SDL_APP_CONTINUE;
        }

        void close()
        {
            closeScene();

            render_.close();

            context_.quit();

            SDL_Quit();
        }

        SDL_AppResult updateEvents(const SDL_Event *event)
        {
            if (event->type == SDL_EVENT_QUIT)
                return SDL_APP_SUCCESS;
            bus_.publish(event);
            return render_.isOpen() ? SDL_APP_CONTINUE : SDL_APP_SUCCESS;
        }

        SDL_AppResult iterate()
        {
            if (!render_.isOpen())
                return SDL_APP_SUCCESS;

            SDL_AppResult res = processIterateResult(curentScene_->update());
            context_.context->Update();

            render_.clear();

            curentScene_->drawScene();
            context_.context->Render();

            render_.display();

            return res;
        }

    private:
        SceneGenerator generator_;

        rmluiwrapper::Context context_;
        sdlwrapper::RenderWindow render_;
        sdlwrapper::EventBus bus_;

        SceneUPtr curentScene_ = nullptr;

    private:
        void closeScene()
        {
            curentScene_.reset();
        }

        SDL_AppResult processIterateResult(const IterateResult &result)
        {
            if (result.getType() == IterateResultType::close)
                return SDL_APP_SUCCESS;
            if (result.getType() == IterateResultType::newScene)
            {
                closeScene();
                curentScene_ = generator_.generateScene(result.getNextSceneId());
                return SDL_APP_CONTINUE;
            }
            return SDL_APP_CONTINUE;
        }
    };

} // namespace engine
