#pragma once

#include "../Engine/Engine.hpp"

#define MENU_SCENE_RML_FILE "ui/MainMenu.html"

namespace engine
{

    class MainSceneListener : public Rml::EventListener
    {
    public:
        MainSceneListener(sdlwrapper::RenderWindow &render, rmluiwrapper::Context &context) : render_(render), context_{context} {}

        void ProcessEvent(Rml::Event &ev) override
        {
            // auto id = ev.GetId();
            Rml::Element *el = ev.GetTargetElement();

            // ID из атрибута id="..."
            const Rml::String &id = el->GetId();

            if (id == "my_text_input")
            {
                if (ev == Rml::EventId::Focus)
                    render_.startTextInput();
                else if (ev == Rml::EventId::Blur)
                    render_.stopTextInput();
            }
            else if (id == "my_button")
            {
                if (ev == Rml::EventId::Click)
                {
                    auto *doc = context_.getDocument(MENU_SCENE_RML_FILE);
                    auto *input_el = doc->GetElementById("my_text_input");
                    auto *out_el = doc->GetElementById("output_text");
                    if (auto *input = rmlui_dynamic_cast<Rml::ElementFormControlInput *>(input_el))
                    {
                        Rml::String text = input->GetValue(); // <-- value из input
                        if (out_el)
                            out_el->SetInnerRML(text); // <-- показываем
                    }
                }
            }
        }

    private:
        sdlwrapper::RenderWindow &render_;
        rmluiwrapper::Context &context_;
    };

    class MenuScene : public Scene
    {
    public:
        MenuScene(sdlwrapper::RenderWindow &render, sdlwrapper::EventBus &bus, rmluiwrapper::Context &context) : Scene(render, bus, context), listener_(render, context)
        {
            Rml::ElementDocument *doc = context.addDocument(MENU_SCENE_RML_FILE);
//            if (auto *input = doc->GetElementById("my_text_input"); input)
//                input->AddEventListener("focus", &listener_);
//            if (auto *button = doc->GetElementById("my_button"); button)
//                button->AddEventListener("click", &listener_);
        }

        void drawScene() const override
        {
        }

        void updateEvents(const SDL_Event *event) override
        {
        }

        IterateResult update() override
        {
            return updateRes_;
        }

    private:
        MainSceneListener listener_;

    private:
    };

} // namespace engine
