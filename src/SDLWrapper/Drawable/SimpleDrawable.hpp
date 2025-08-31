#ifndef SDL_WRAPPER_SIMPLE_DRAWABLE_HPP
#define SDL_WRAPPER_SIMPLE_DRAWABLE_HPP

#include <SDL3/SDL.h>

namespace sdlwrapper
{
    class SimpleDrawable
    {
    public:
        friend class RenderWindow;
    protected:
        virtual void draw(SDL_Renderer* renderer) const = 0;
    };

} // namespace sdlwrapper


#endif