#ifndef SDL_WRAPPER_FINGER_CLICKABLE_HPP
#define SDL_WRAPPER_FINGER_CLICKABLE_HPP

#include <limits>

#include <SDL3/SDL.h>

#include "ClickableActionType.hpp"

namespace sdlwrapper
{

    class FingerClickable
    {
    public:
        virtual ~FingerClickable() = default;

        void setWindowSize(const SDL_Point& windowSize)
        {
            windowSize_ = windowSize;
        }

    protected:

        bool isPressedNow_ = false;
        SDL_Point windowSize_ = {};
        SDL_FingerID fingerId_ = std::numeric_limits<SDL_FingerID>::max();

    protected:

        virtual bool contains(const float x, const float y) const = 0;

        ClickableActionType updateFingerClick(const SDL_Event* event)
        {
            if(!isFingerClickEvent(event))
                return ClickableActionType::none;
            bool contain = contains(event->tfinger.x * windowSize_.x, event->tfinger.y * windowSize_.y);
            if(event->type == SDL_EVENT_FINGER_DOWN)
            {
                if(!contain || fingerId_ != std::numeric_limits<SDL_FingerID>::max())
                    return ClickableActionType::none;
                fingerId_ = event->tfinger.fingerID;
                isPressedNow_ = true;
                return ClickableActionType::pressed;
            }
            else if(event->type == SDL_EVENT_FINGER_UP)
            {
                if(fingerId_ != event->tfinger.fingerID)
                    return ClickableActionType::none;
                fingerId_ = std::numeric_limits<SDL_FingerID>::max();
                ClickableActionType res = (contain && isPressedNow_) ? ClickableActionType::clicked : ClickableActionType::none;
                isPressedNow_ = false;
                return res;
            }
            else // if(event->type == SDL_EVENT_FINGER_MOTION)
            {
                if(fingerId_ != event->tfinger.fingerID)
                    return ClickableActionType::none;
                if(!contain && isPressedNow_)
                {
                    isPressedNow_ = false;
                    return ClickableActionType::released;
                }
                return ClickableActionType::none;
            }
        }

        inline static bool isFingerClickEvent(const SDL_Event* event)
        {
            return event->type == SDL_EVENT_FINGER_DOWN || event->type == SDL_EVENT_FINGER_UP || event->type == SDL_EVENT_FINGER_MOTION;
        }

    };
    
} // namespace sdlwrapper

#endif //SDL_WRAPPER_FINGER_CLICKABLE_HPP