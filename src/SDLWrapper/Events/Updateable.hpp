#ifndef SDL_WRAPPER_UPDATEABLE_HANDLER
#define SDL_WRAPPER_UPDATEABLE_HANDLER

#include <SDL3/SDL.h>

namespace sdlwrapper
{
    class Updateable
    {   
    public:
        virtual ~Updateable() = default;
    
        virtual void updateEvents(const SDL_Event* event) = 0;

    };

} // namespace sdlwrapper

#endif