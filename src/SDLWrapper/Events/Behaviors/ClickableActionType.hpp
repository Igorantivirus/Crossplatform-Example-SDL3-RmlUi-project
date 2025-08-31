#ifndef SDL_WRAPPER_CLICKABLE_ACTIOM_TYPE_HPP
#define SDL_WRAPPER_CLICKABLE_ACTIOM_TYPE_HPP

namespace sdlwrapper
{
    enum class ClickableActionType : unsigned char
    {
        none = 0,   //состояние не изменилось
        pressed,    //отжата
        released,   //зажата
        clicked     //кликнута
    };
} // namespace sdlwrapper


#endif //SDL_WRAPPER_CLICKABLE_ACTIOM_TYPE_HPP