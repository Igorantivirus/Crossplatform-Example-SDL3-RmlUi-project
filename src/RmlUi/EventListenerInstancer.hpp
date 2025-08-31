#pragma once

#include <RmlUi/Core.h>

namespace rmluiwrapper
{
    class EventFabrick : public Rml::EventListenerInstancer
    {
    public:
        Rml::EventListener *InstanceEventListener(const Rml::String &value, Rml::Element *element) override
        {
            // Rml::EventListener *listener = new ElementsEventListener{value, element};
            return nullptr;
        }
    };
} // namespace rmluiwrapper
