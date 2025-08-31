#pragma once

#include <map>
#include <string>

#include <RmlUi/Core.h>

#ifndef NDEBUG
#include <RmlUi/Debugger.h>
#endif

#include "Types.hpp"
#include "EventListenerInstancer.hpp"

#include "../SDLWrapper/SDLWrapper.hpp"

namespace rmluiwrapper
{

    struct Context : public sdlwrapper::Updateable
    {
        Context() = default;
        Context(const Context &other) = delete;
        Context(Context &&other) = default;

        Rml::Context *context = nullptr;

        std::map<std::string, Rml::ElementDocument *> documents;

        bool initRmlUi(sdlwrapper::RendererSPtr renderer, sdlwrapper::WindowSPtr window, const std::string &fontsPath)
        {
            window_ = window;

            rendrInterface_ = std::make_unique<RenderInterface_SDL>(renderer.get());
            systemInterface_ = std::make_unique<SystemInterface_SDL>();
            fileInterface_ = std::make_unique<FileInterface>();

            // rendrInterface_ = new RenderInterface_SDL{renderer.get()}; // std::make_unique<RenderInterface_SDL>(renderer.get());
            // systemInterface_ = new SystemInterface_SDL;                // std::make_unique<SystemInterface_SDL>();
            // fileInterface_ = new FileInterface;                        // std::make_unique<FileInterface>();

            Rml::SetRenderInterface(rendrInterface_.get());
            Rml::SetSystemInterface(systemInterface_.get());
            Rml::SetFileInterface(fileInterface_.get());

            if (!Rml::Initialise())
            {
                SDL_Log("Rml::Initialise failed");
                return false;
            }

            Rml::Factory::RegisterEventListenerInstancer(&eventFabrick_);

            if (!SDL_GetWindowSize(window.get(), &wSize_.x, &wSize_.y))
            {
                SDL_Log("SDL_GetWindowSize failed! Error: %s", SDL_GetError());
                return false;
            }

            context = Rml::CreateContext("main", Rml::Vector2i(wSize_.x, wSize_.y));
            if (!context)
            {
                SDL_Log("Rml::CreateContext failed");
                return false;
            }
            context->SetDensityIndependentPixelRatio(SDL_GetWindowDisplayScale(window.get()));

#ifndef NDEBUG
            Rml::Debugger::Initialise(context);
#endif
            return loadFonts(fontsPath);
        }

        void quit()
        {
            if (context)
            {
                for (auto &[id, doc] : documents)
                    if (doc)
                        doc->Close();
                documents.clear();

                auto name = context->GetName();
                Rml::RemoveContext(name);
                context = nullptr;
            }
            Rml::Shutdown();
            // delete rendrInterface_;
            // delete systemInterface_;
            // delete fileInterface_;
        }

        void updateEvents(const SDL_Event *event) override
        {
            if (event->type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED || event->type == SDL_EVENT_WINDOW_RESIZED || event->type == SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED || event->type == SDL_EVENT_DISPLAY_ORIENTATION)
                updateDimensions();
            else if (event->type == SDL_EVENT_MOUSE_MOTION)
                context->ProcessMouseMove(event->motion.x, event->button.y, 0);
            else if (event->type == SDL_EVENT_TEXT_INPUT)
                context->ProcessTextInput(event->text.text);
            else if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
                context->ProcessMouseButtonDown(0, 0);
            else if (event->type == SDL_EVENT_MOUSE_BUTTON_UP)
                context->ProcessMouseButtonUp(0, 0);
            else if (event->type == SDL_EVENT_KEY_DOWN)
                context->ProcessKeyDown(RmlSDL::ConvertKey(event->key.key), 0);
        }

        Rml::ElementDocument *addDocument(const std::string &path)
        {
            Rml::ElementDocument *doc = context->LoadDocument(path);
            if (!doc)
                return nullptr;
            else
                doc->Show();
            documents[path] = doc;
            return doc;
        }

        Rml::ElementDocument *getDocument(const std::string &path)
        {
            auto found = documents.find(path);
            return found == documents.end() ? nullptr : found->second;
        }

    private:
        RenderInterfaceUPtr rendrInterface_ = nullptr;
        SystemInterfaceUPtr systemInterface_ = nullptr;
        FileInterfaceUptr fileInterface_ = nullptr;

        // RenderInterface_SDL *rendrInterface_ = nullptr;
        // SystemInterface_SDL *systemInterface_ = nullptr;
        // FileInterface *fileInterface_ = nullptr;

        EventFabrick eventFabrick_;

        sdlwrapper::WindowWPtr window_;
        SDL_Point wSize_ = {};

        bool updateDimensions()
        {
            SDL_Window* window = window_.lock().get();
            if(!window)
                return false;
            if (!SDL_GetWindowSize(window, &wSize_.x, &wSize_.y))
            {
                SDL_Log("SDL_GetWindowSize failed! Error: %s", SDL_GetError());
                return false;
            }
            context->SetDimensions(Rml::Vector2i(wSize_.x, wSize_.y));
            context->SetDensityIndependentPixelRatio(SDL_GetWindowDisplayScale(window));
            return true;
        }

    private:

        bool loadFonts(const std::string &fontsPath)
        {
            std::string strFile;
            {
                using namespace sdlwrapper;
                FileWorker file(fontsPath, FileWorkerMode::read | FileWorkerMode::binary);
                if (!file.isOpen())
                    return false;
                strFile = file.readAll();
            }
            strFile += '\n';

            for (std::size_t cur = strFile.find('\n'), last = 0; cur != std::string::npos; last = cur, cur = strFile.find('\n', last + 1))
            {
                std::string pr = strFile.substr(last, cur - last);
                SDL_Log("Next: \"%s\"", pr.c_str());
                bool fallback = (pr.find("Emoji") != std::string::npos);
                if (!Rml::LoadFontFace(pr, fallback))
                {
                    SDL_Log("Failed to load font: %s", pr.c_str());
                    return false;
                }
            }
            return true;
        }

    };

} // namespace rmluiwrapper
