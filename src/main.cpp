#ifndef SDL_MAIN_USE_CALLBACKS
#define SDL_MAIN_USE_CALLBACKS
#endif

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Engine/Engine.hpp"

static engine::Engine app;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    return app.start(1, "My Example");
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    return app.updateEvents(event);
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    return app.iterate();
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    app.close();
}