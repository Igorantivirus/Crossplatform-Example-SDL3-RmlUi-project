#ifndef SDL_WRAPPER_DRAWABLE_HPP
#define SDL_WRAPPER_DRAWABLE_HPP

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "SimpleDrawable.hpp"

namespace sdlwrapper
{

    class Drawable : public SimpleDrawable
    {
    public:

        #pragma region init
        
        Drawable() = default;
        Drawable(Drawable&& other) = default;
        Drawable(const Drawable& other) = default;
        virtual ~Drawable() = default;
        
        Drawable& operator=(Drawable&& other) = default;
        Drawable& operator=(const Drawable& other) = default;

        #pragma endregion
    
        #pragma region set

        virtual void setPosition(const SDL_FPoint& position)
        {
            distRect_.x = position.x;
            distRect_.y = position.y;
        }
        virtual void setPosition(const float x, const float y)
        {
            distRect_.x = x;
            distRect_.y = y;
        }
        virtual void move(const SDL_FPoint& position)
        {
            distRect_.x += position.x;
            distRect_.y += position.y;
        }
        virtual void move(const float x, const float y)
        {
            distRect_.x += x;
            distRect_.y += y;
        }
        virtual void setAngle(const float angle)
        {
            angle_ = angle;
        }
        virtual void setScale(const float scale)
        {
            scale_ = scale;
        }
        void setVisible(const bool visible)
        {
            isvisible_ = visible;
        }

        #pragma endregion

        #pragma region get

        virtual SDL_FPoint getPosition() const
        {
            return SDL_FPoint{distRect_.x, distRect_.y};
        }
        virtual float getScale() const
        {
            return scale_;
        }
        virtual float getAngle() const
        {
            return angle_;
        }
        virtual SDL_FRect getScreenRect() const
        {
            return distRect_;
        }
        bool isVisible() const
        {
            return isvisible_;
        }

        #pragma endregion

    protected:

        float      scale_   = {1.f};
        float      angle_   = {0.f};

        SDL_FRect distRect_{};

        bool isvisible_ = true;

    protected:

        virtual void draw(SDL_Renderer* renderer) const override = 0;

        bool modifiedDrawable() const
        {
            return scale_ != 1.f || angle_ != 0.f;
        }

    };

} // sdlwrapper

#endif //SDL_WRAPPER_DRAWABLE_HPP
