#include <SDL3/SDL_events.h>
#include <SDL3/SDL_main.h>
#include <SDLWrapper/Math/Colors.hpp>
#include <SDLWrapper/SDLWrapper.hpp>

sdl3::RenderWindow window;
sdl3::View view;

sdl3::Texture texture;
sdl3::EllipseShape elip;
sdl3::Sprite sprite1;
sdl3::Sprite sprite2;
sdl3::RectangleShape rect;
sdl3::CircleShape circ;



SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    
    // SDL_SharedObject
    // SDL_SetRenderScale(SDL_Renderer *renderer, float scaleX, float scaleY)

    sdl3::VideoMode mode = sdl3::VideoMode::getDefaultVideoMode();
    mode.fullscreen = false;
    mode.width /= 2.f;
    mode.height /= 2.f;
    bool res = window.create("window", mode);
    window.loadIconFromFile("ABS.png");
    window.setLogicalPresentation({800,800});

    texture.loadFromFile("ABS.png");

    sprite1.setTexture(texture);
    sprite1.setPosition({0, 0});
    sprite1.setUniformScale(2.f);
    sprite1.setOriginKeepPosition({texture.getSize().x / 2.f, texture.getSize().y / 2.f});
    sprite1.setFilterColor(sdl3::Colors::Blue);

    sprite2.setTexture(texture);
    sprite2.setPosition({100, 100});
    
    sprite2.setOriginKeepPosition(sdl3::Vector2f{texture.getSize().x / 2.f, texture.getSize().y / 2.f});

    rect.setSize({200.f, 120.f});
    rect.setFillColor(sdl3::Colors::Red);
    rect.setTexture(texture);
    rect.setPosition({300.f, 200.f});
    rect.setUniformScale(2);
    rect.setRotation(-45);

    circ.setRadius(50);
    circ.setTexture(texture);
    circ.setPosition({500, 200});
    circ.setOutlineColor(sdl3::Colors::Green);
    circ.setOutlineThickness(10);
    circ.setPointCount(2);

    elip.setPosition({600, 300});
    elip.setRadii({50, 70});
    elip.setFillColor(sdl3::Colors::Black);
    elip.setOutlineColor(sdl3::Colors::Red);
    elip.setOutlineThickness(5);
    elip.setTexture(texture);
    elip.setTextureRect({10, 10, 35, 35});

    view = window.getView();
    // view.setAngle(150);
    // view.setAngle(0);
    // window.setView(view);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        window.close();
        return SDL_APP_SUCCESS;
    }

    if(event->type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        window.convertEventToRenderCoordinates(event);
        SDL_Log("%f %f\n", event->button.x, event->button.y);
    }
    // return SDL_APP_CONTINUE;
    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        if (event->key.key == SDLK_ESCAPE)
        {
            window.close();
            return SDL_APP_SUCCESS;
        }
        if (event->key.key == SDLK_L)
        {
            // SDL_Log("ABOBA {} {} {}", "negr", 2, 3.1415);
            // error("ABOBA");
            //  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s", "ABOBA");
            return SDL_APP_CONTINUE;
        }
        if (event->key.key == SDLK_M)
        {
            // LOG_ERROR("Пользователь, гад такой, нажал {}!!! Наказание: {}", "M", "Палка в дупу");
            // error("ABOBA");
            //  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s", "ABOBA");
            return SDL_APP_CONTINUE;
        }
        if (event->key.key == SDLK_N)
        {
            window.setFullScreen(!window.isFullScreen());
            return SDL_APP_CONTINUE;
        }
        sdl3::Vector2f pos = {};
        if (event->key.key == SDLK_W)
            pos.y -= 1;
        else if (event->key.key == SDLK_A)
            pos.x -= 1;
        else if (event->key.key == SDLK_S)
            pos.y += 1;
        else if (event->key.key == SDLK_D)
            pos.x += 1;
        if (pos.x != 0.f || pos.y != 0.f)
        {
            sprite2.move(pos);
            
            return SDL_APP_CONTINUE;
        }

        if (event->key.key == SDLK_UP)
            pos.y -= 5;
        else if (event->key.key == SDLK_LEFT)
            pos.x -= 5;
        else if (event->key.key == SDLK_DOWN)
            pos.y += 5;
        else if (event->key.key == SDLK_RIGHT)
            pos.x += 5;

        if (pos.x != 0.f || pos.y != 0.f)
        {
            sdl3::Vector2f center = view.getCenterPosition();
            view.setCenterPosition({center.x + pos.x, center.y + pos.y});
            window.setView(view);
            return SDL_APP_CONTINUE;
        }

        if (event->key.scancode == SDL_SCANCODE_KP_8)
            sprite2.rotate(1);
        else if (event->key.scancode == SDL_SCANCODE_KP_2)
            sprite2.rotate(-1);
        else
        {
            if (event->key.scancode == SDL_SCANCODE_KP_4)
            {
                view.rotate(1);
                window.setView(view);
            }
            else if (event->key.scancode == SDL_SCANCODE_KP_6)
            {
                view.rotate(-1);
                window.setView(view);
            }
            else if (event->key.scancode == SDL_SCANCODE_KP_PLUS)
            {
                // SDL_FPoint scale = view.getZoom();
                // scale.x *= 1.1f;
                // scale.y *= 1.1f;
                // view.setZoom(scale);
                // window.setView(view);

                sdl3::Vector2f scale = sprite2.getScale();
                scale.x *= 1.1f;
                scale.y *= 1.1f;
                sprite2.setScale(scale);
            }
            else if (event->key.scancode == SDL_SCANCODE_KP_MINUS)
            {
                // SDL_FPoint scale = view.getZoom();
                // scale.x /= 1.1f;
                // scale.y /= 1.1f;
                // view.setZoom(scale);
                // window.setView(view);

                sdl3::Vector2f scale = sprite2.getScale();
                scale.x /= 1.1f;
                scale.y /= 1.1f;
                sprite2.setScale(scale);
            }
        }
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    window.clear(sdl3::Colors::White);
    window.draw(sprite1);
    window.draw(sprite2);
    window.draw(rect);
    window.draw(circ);
    window.draw(elip);
    window.display();
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    window.close();
}
