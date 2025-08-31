#ifndef SDL_WRAPPER_EVENT_BUS
#define SDL_WRAPPER_EVENT_BUS

#include <set>
#include <map>
#include <algorithm>
#include <initializer_list>

#include "Updateable.hpp"

namespace sdlwrapper
{

    class EventBus
    {
    public:
        EventBus() = default;

        void subscribe(Updateable& handler)
        {
            subers_.insert(&handler);
        }
        void subscribe(Updateable** handlers, const std::size_t size)
        {
            for(std::size_t i = 0; i < size; ++i)
                subers_.insert(handlers[i]);
        }
        void subscribe(const std::initializer_list<Updateable*> handlers)
        {
            for(const auto& i : handlers)
                subers_.insert(i);
        }
        
        void unsubscribe(Updateable& handler)
        {
            subers_.erase(&handler);
        }
        void unsubscribe(Updateable** handlers, const std::size_t size)
        {
            for(std::size_t i = 0; i < size; ++i)
                subers_.erase(handlers[i]);
        }
        void unsubscribe(const std::initializer_list<Updateable*> handlers)
        {
            for(const auto& i : handlers)
                subers_.erase(i);
        }
        
        bool isSigned(Updateable& handler) const
        {
            return static_cast<bool>(subers_.count(&handler));
        }

        void publish(const SDL_Event* event)
        {
            for(auto& i : subers_)
                i->updateEvents(event);
        }

    private:

        std::set<Updateable*> subers_;

    };

} // namespace sdlwrapper


#endif //SDL_WRAPPER_EVENT_BUS