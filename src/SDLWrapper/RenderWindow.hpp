#ifndef KNIGHTS_AND_CASTLES_RENDERWINDOW_HPP
#define KNIGHTS_AND_CASTLES_RENDERWINDOW_HPP

#include <string>

#include <SDL3/SDL.h>

#include "VideoMode.hpp"
#include "Texture.hpp"
#include "Drawable/SimpleDrawable.hpp"
#include "AdditionalSyntaxis.hpp"

namespace sdlwrapper
{

    class RenderWindow
    {
    public:
        RenderWindow()
        {
            // safelySdlInit();
        }
        RenderWindow(const RenderWindow &other) = delete;
        RenderWindow(RenderWindow &&other) = delete;
        ~RenderWindow()
        {
            // safelySdlQuit();
        }
        RenderWindow &operator=(const RenderWindow &) = delete;

        void connectTextureToRender(Texture &texture) const
        {
            texture.renderer_ = renderer_;
        }

        bool openWindow(const VideoMode &mode, const std::string &windowName)
        {
            SDL_Window *window;
            SDL_Renderer *renderer;
            if (!SDL_CreateWindowAndRenderer(windowName.c_str(), mode.resolution.x, mode.resolution.y, mode.flags, &window, &renderer))
            {
                SDL_Log("Fatal SDL_CreateWindowAndRenderer. Error: %s", SDL_GetError());
                return isOpen_ = false;
            }

            renderer_.reset(renderer, SDL_DestroyRenderer);
            window_.reset(window, SDL_DestroyWindow);

            SDL_ShowWindow(window);
            SDL_SetRenderVSync(renderer, -1);
            return isOpen_ = true;
        }
        void close()
        {
            renderer_.reset();
            window_.reset();
            isOpen_ = false;
        }

        bool isOpen() const
        {
            return isOpen_;
        }

        void setClearColor(const SDL_Color &color)
        {
            clearColor_ = color;
            SDL_SetRenderDrawColor(renderer_.get(), clearColor_.r, clearColor_.g, clearColor_.b, SDL_ALPHA_OPAQUE);
        }

        WindowSPtr getNativeWindow()
        {
            return window_;
        }
        RendererSPtr getNativeRender()
        {
            return renderer_;
        }
        SDL_Point getWindowSize() const
        {
            int window_width, window_height;
            SDL_GetWindowSize(window_.get(), &window_width, &window_height);
            return {window_width, window_height};
        }
        SDL_Color getClearColor() const
        {
            return clearColor_;
        }

        void startTextInput()
        {
            SDL_StartTextInput(window_.get());
        }
        void stopTextInput()
        {
            SDL_StopTextInput(window_.get());
        }

        void clear()
        {
            SDL_SetRenderDrawColor(renderer_.get(), clearColor_.r, clearColor_.g, clearColor_.b, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer_.get());
        }
        void draw(const SimpleDrawable &object)
        {
            object.draw(renderer_.get());
        }

        void display()
        {
            SDL_RenderPresent(renderer_.get());
        }

    private:
        WindowSPtr window_ = nullptr;
        RendererSPtr renderer_ = nullptr;

        bool isOpen_ = false;

        SDL_Color clearColor_ = {0, 0, 0, 255};

    private: // static
        // static uint8_t initializedCount_;

        // static bool safelySdlInit()
        // {
        //     if (initializedCount_ != 0)
        //         return true;
        //     if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
        //     {
        //         SDL_Log("SDL_Init Error: %s", SDL_GetError());
        //         return false;
        //     }
        //     ++initializedCount_;
        //     return true;
        // }
        // static void safelySdlQuit()
        // {
        //     if (initializedCount_ == 0)
        //         return;
        //     --initializedCount_;
        //     if (initializedCount_ == 0)
        //         SDL_Quit();
        // }
    };

    // uint8_t RenderWindow::initializedCount_ = 0;

} // sdlwrapper

#endif // KNIGHTS_AND_CASTLES_RENDERWINDOW_HPP
