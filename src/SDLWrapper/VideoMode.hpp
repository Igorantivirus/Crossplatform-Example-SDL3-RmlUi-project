#ifndef KNIGHTS_AND_CASTLES_VIDEOMODE_HPP
#define KNIGHTS_AND_CASTLES_VIDEOMODE_HPP

#include <string>

#include <SDL3/SDL.h>

namespace sdlwrapper {

    struct VideoMode
    {
        SDL_Point resolution  = {};
        int flags = {};

        static VideoMode defaultMode()
        {
            // Получаем информацию о основном дисплее (индекс 0)
            SDL_DisplayID display = SDL_GetPrimaryDisplay();
            SDL_Rect display_bounds;
            if (!SDL_GetDisplayBounds(display, &display_bounds))
                return {};

            VideoMode res;
            res.resolution.x = display_bounds.w / 2;
            res.resolution.y = display_bounds.h / 2;
            res.flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
            return res;
        }

    };

} // sdlwrapper

#endif //KNIGHTS_AND_CASTLES_VIDEOMODE_HPP
