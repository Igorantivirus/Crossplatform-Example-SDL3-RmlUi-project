#ifndef SDL_WRAPPER_MOUSE_MOVABLE_HANDLER
#define SDL_WRAPPER_MOUSE_MOVABLE_HANDLER

#include <limits>

#include <SDL3/SDL.h>

namespace sdlwrapper
{
    class MouseMovable
    {
    public:
        virtual ~MouseMovable() = default;

    protected:

        bool isCapturedNow_ = false;

    protected:

        virtual bool contains(const float x, const float y) const = 0;

        SDL_FPoint updateMouseMove(const SDL_Event* event)
        {
            if(!isMouseMoveEvent(event))
                return {};

            if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                if(!contains(event->button.x, event->button.y))
                    return {};
                isCapturedNow_ = true;
                return {};
            }
            else if(event->type == SDL_EVENT_MOUSE_BUTTON_UP)
            {
                isCapturedNow_ = false;
                return {};
            }
            else // if(event->type == SDL_EVENT_MOUSE_MOTION)
            {
                return isCapturedNow_ ? SDL_FPoint{event->motion.xrel, event->motion.yrel} : SDL_FPoint{};
            }

        }

        inline static bool isMouseMoveEvent(const SDL_Event* event)
        {
            return 
            ((event->type == SDL_EVENT_MOUSE_BUTTON_DOWN || event->type == SDL_EVENT_MOUSE_BUTTON_UP) && (event->button.button == SDL_BUTTON_LEFT))
            || event->type == SDL_EVENT_MOUSE_MOTION;
        }

    };

} // namespace sdlwrapper

#endif //SDL_WRAPPER_MOUSE_MOVABLE_HANDLER