#ifndef SDL_WRAPPER_MOUSE_CLICKABLE_HPP
#define SDL_WRAPPER_MOUSE_CLICKABLE_HPP

#include <SDL3/SDL.h>

#include "ClickableActionType.hpp"

namespace sdlwrapper
{
    class MouseClickable
    {
    public:
        virtual ~MouseClickable() = default;

    protected:

        bool isPressedNow_ = false;

    protected:

        virtual bool contains(const float x, const float y) const = 0;

        ClickableActionType updateMouseClick(const SDL_Event* event)
        {
            if(!isMouseClickEvent(event))
                return ClickableActionType::none;

            bool contain = contains(event->button.x, event->button.y);
            if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                isPressedNow_ = contain;
                return contain ? ClickableActionType::pressed : ClickableActionType::none;
            }
            else if(event->type == SDL_EVENT_MOUSE_BUTTON_UP)
            {
                ClickableActionType res = (contain && isPressedNow_) ? ClickableActionType::clicked : ClickableActionType::none;
                isPressedNow_ = false;
                return res;
            }
            else // if(event->type == SDL_EVENT_MOUSE_MOTION)
            {
                if(!contain && isPressedNow_)
                {
                    isPressedNow_ = false;
                    return ClickableActionType::released;
                }
                return ClickableActionType::none;
            }
        }

        inline static bool isMouseClickEvent(const SDL_Event* event)
        {
            return 
            ((event->type == SDL_EVENT_MOUSE_BUTTON_DOWN || event->type == SDL_EVENT_MOUSE_BUTTON_UP) && (event->button.button == SDL_BUTTON_LEFT))
            || event->type == SDL_EVENT_MOUSE_MOTION;
        }

    };
    
} // namespace sdlwrapper

#endif //SDL_WRAPPER_MOUSE_CLICKABLE_HPP