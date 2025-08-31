#ifndef SDL_WRAPPER_ADDITIONAL_SYNTAXIS
#define SDL_WRAPPER_ADDITIONAL_SYNTAXIS

#include <cmath>
#include <memory>

#include <SDL3/SDL.h>

namespace sdlwrapper
{

    struct SDLDeleter
    {
        inline void operator()(SDL_Texture* texture) const noexcept
        {
            if (texture)
                SDL_DestroyTexture(texture);
        }
        inline void operator()(SDL_Window* window) const noexcept
        {
            if (window)
                SDL_DestroyWindow(window);
        }
        inline void operator()(SDL_Renderer* renderer) const noexcept
        {
            if (renderer)
                SDL_DestroyRenderer(renderer);
        }
    };
    using TextureSPtr  = std::shared_ptr<SDL_Texture>;
    using WindowSPtr   = std::shared_ptr<SDL_Window>;
    using RendererSPtr = std::shared_ptr<SDL_Renderer>;

    using TextureUPtr  = std::unique_ptr<SDL_Texture,  SDLDeleter>;
    using WindowUPtr   = std::unique_ptr<SDL_Window,   SDLDeleter>;
    using RendererUPtr = std::unique_ptr<SDL_Renderer, SDLDeleter>;

    using TextureWPtr  = std::weak_ptr<SDL_Texture>;
    using WindowWPtr   = std::weak_ptr<SDL_Window>;
    using RendererWPtr = std::weak_ptr<SDL_Renderer>;

    using SDLEventType = decltype(SDL_Event::type);

    bool rectContains(const SDL_FRect& rect, const float x, const float y, const float angle = 0)
    {
        if(angle == 0.f)
            return x >= rect.x && y >= rect.y && x <= (rect.x + rect.w) && y <= (rect.y + rect.h);
        /*------------- 1. центр и полуразмеры прямоугольника -------------*/
        const float halfW = rect.w * 0.5f;
        const float halfH = rect.h * 0.5f;
        const float cx    = rect.x + halfW;   // pivot (центр вращения)
        const float cy    = rect.y + halfH;

        /*------------- 2. переводим точку в систему координат центра -------------*/
        float dx = x - cx;
        float dy = y - cy;

        #define MY_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679f

        /*------------- 3. если прямоугольник повернут – «открутим» точку назад -------------*/
            const float rad  = angle * MY_PI / 180.f; // угол в радианах
            const float c    = std::cos(rad);
            const float s    = std::sin(rad);

            // Поворот на -angle:  (dx,dy) → (dx',dy')
            const float dx2 =  dx * c + dy * s;
            const float dy2 = -dx * s + dy * c;
            dx = dx2;
            dy = dy2;

        /*------------- 4. проверка «в пределах половины ширины / высоты» -------------*/
        return std::abs(dx) <= halfW && std::abs(dy) <= halfH;
    }
    bool rectContains(const SDL_FRect& rect, const SDL_FPoint& position, const float angle = 0)
    {
        return rectContains(rect, position.x, position.y, angle);
    }

    SDL_FPoint rotateVector(const SDL_FPoint& vec, const float fi)
    {
        const float sin_ = std::sin(fi);
        const float cos_ = std::cos(fi);
        return SDL_FPoint
        {
            vec.x * cos_ - vec.y * sin_,
            vec.x * sin_ + vec.y * cos_
        };
    }

    SDL_FPoint operator+(const SDL_FPoint& p1, const SDL_FPoint& p2)
    {
        return SDL_FPoint{p1.x + p2.x, p1.y + p2.y};
    }
    SDL_FPoint operator-(const SDL_FPoint& p1, const SDL_FPoint& p2)
    {
        return SDL_FPoint{p1.x - p2.x, p1.y - p2.y};
    }
    SDL_FPoint operator*(const SDL_FPoint& p, const float value)
    {
        return SDL_FPoint{p.x * value, p.y * value};
    }
    SDL_FPoint operator*(const float value, const SDL_FPoint& p)
    {
        return SDL_FPoint{p.x * value, p.y * value};
    }
    SDL_FPoint operator/(const SDL_FPoint& p, const float value)
    {
        return SDL_FPoint{p.x / value, p.y / value};
    }
    SDL_FPoint operator/(const float value, const SDL_FPoint& p)
    {
        return SDL_FPoint{p.x / value, p.y / value};
    }
    SDL_FPoint& operator+=(SDL_FPoint& p1, const SDL_FPoint& p2)
    {
        p1.x += p2.x;
        p1.y += p2.y;
        return p1;
    }
    SDL_FPoint& operator-=(SDL_FPoint& p1, const SDL_FPoint& p2)
    {
        p1.x -= p2.x;
        p1.y -= p2.y;
        return p1;
    }
    SDL_FPoint& operator*=(SDL_FPoint& p, const float value)
    {
        p.x *= value;
        p.y *= value;
        return p;
    }
    SDL_FPoint& operator/=(SDL_FPoint& p, const float value)
    {
        p.x /= value;
        p.y /= value;
        return p;
    }

} // namespace sdlwrapper



#endif //SDL_WRAPPER_ADDITIONAL_SYNTAXIS