#ifndef SDL_WRAPPER_FINGER_MOVABLE_HANDLER
#define SDL_WRAPPER_FINGER_MOVABLE_HANDLER

#include <limits>

#include <SDL3/SDL.h>

namespace sdlwrapper
{
    class FingerMovable
    {
    public:
        virtual ~FingerMovable() = default;

        void setWindowSize(const SDL_Point& windowSize)
        {
            windowSize_ = windowSize;
        }

    protected:

        bool isCapturedNow_ = false;
        SDL_Point windowSize_ = {};
        SDL_FingerID fingerId_ = std::numeric_limits<SDL_FingerID>::max();

    protected:

        virtual bool contains(const float x, const float y) const = 0;

        SDL_FPoint updateFingerMove(const SDL_Event* event)
        {
            if(!isMouseMoveEvent(event))
                return {};

            if(event->type == SDL_EVENT_FINGER_DOWN)
            {
                if(fingerId_ != std::numeric_limits<SDL_FingerID>::max())
                    return {};
                if(!contains(event->tfinger.x * windowSize_.x, event->tfinger.y * windowSize_.y))
                    return {};
                isCapturedNow_ = true;
                fingerId_ = event->tfinger.fingerID;
                return {};
            }
            else if(event->type == SDL_EVENT_FINGER_UP)
            {
                if(fingerId_ != event->tfinger.fingerID)
                    return {};
                fingerId_ = std::numeric_limits<SDL_FingerID>::max();
                isCapturedNow_ = false;
                return {};
            }
            else // if(event->type == SDL_EVENT_FINGER_MOTION)
            {
                if(fingerId_ != event->tfinger.fingerID)
                    return {};
                return isCapturedNow_ ? SDL_FPoint{event->tfinger.dx * windowSize_.x, event->tfinger.dx * windowSize_.y} : SDL_FPoint{};
            }

        }

        inline static bool isMouseMoveEvent(const SDL_Event* event)
        {
            return event->type == SDL_EVENT_FINGER_DOWN || event->type == SDL_EVENT_FINGER_UP || event->type == SDL_EVENT_FINGER_MOTION;
        }

    };

} // namespace sdlwrapper

#endif //SDL_WRAPPER_FINGER_MOVABLE_HANDLER