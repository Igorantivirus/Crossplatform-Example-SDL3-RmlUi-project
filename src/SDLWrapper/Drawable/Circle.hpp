#ifndef KNIGHTS_AND_CASTLES_CIRCLE_HPP
#define KNIGHTS_AND_CASTLES_CIRCLE_HPP

#include <string>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3_gfx/SDL3_gfxPrimitives.h>

#include "../Texture.hpp"
#include "Drawable.hpp"

namespace sdlwrapper
{

    class Circle : public Drawable
    {
    public:
        Circle() = default;

        #pragma region set get

        void setRadius(const float r)
        {
            if(r <= 0)
                return;
            radius_ = r;
            updateRect();
        }
        void setColor(const SDL_Color& color)
        {
            color_ = color;
        }
        void setScale(const float scale) override
        {
            Drawable::setScale(scale);
            updateRect();
        }

        float getRadius() const
        {
            return radius_;
        }
        const SDL_Color& getColor() const
        {
            return color_;
        }

        #pragma endregion

    private:

        float radius_ = 0;
        SDL_Color color_ = {};

    private:

        void draw(SDL_Renderer* renderer) const override
        {
            filledCircleRGBA(renderer, distRect_.x + radius_ * scale_, distRect_.y + radius_ * scale_, radius_, color_.r, color_.g, color_.b, color_.a);
        }

        void updateRect()
        {
            distRect_.w = radius_ * 2 * scale_;
            distRect_.h = radius_ * 2 * scale_;
        }

        // void initPoints()
        // {
        //     points_.clear();
        //     points_.reserve(pointsCount_);
        //     #define MY_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679f
        //     const float r = radius_ * scale_;
        //     const float centerX = distRect_.x + r;
        //     const float centerY = distRect_.y + r;
        //     const float angle = 2 * MY_PI / pointsCount_;
        //     const float sinOfAngle = std::sin(angle);
        //     const float cosOfAngle = std::cos(angle);
        //     SDL_FPoint point = {distRect_.x + 2 * r, distRect_.y + r};
        //     float vecX = 0.f;
        //     float vecY = 0.f;
        //     for(int i = 0; i < pointsCount_; ++i)
        //     {
        //         points_.push_back(point);
        //         vecX = point.x - centerX;
        //         vecY = point.y - centerY;
        //         point.x = centerX + (vecX * cosOfAngle - vecY * sinOfAngle);
        //         point.y = centerY + (vecX * sinOfAngle + vecY * cosOfAngle);
        //     }
        // }

    };

} // sdlwrapper

#endif //KNIGHTS_AND_CASTLES_CIRCLE_HPP
