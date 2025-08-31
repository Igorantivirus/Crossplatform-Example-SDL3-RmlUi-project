#ifndef KNIGHTS_AND_CASTLES_SPRITE_HPP
#define KNIGHTS_AND_CASTLES_SPRITE_HPP

#include <string>

#include <SDL3/SDL.h>

#include "../Texture.hpp"
#include "Drawable.hpp"

namespace sdlwrapper
{

    class Sprite : public  Drawable
    {
    public:
        Sprite() = default;

        #pragma region set

        void setScale(const float scale) override
        {
            Drawable::setScale(scale);
            updateRect();
        }
        void setTexture(const Texture& texture)
        {
            texturePtr_ = &texture;
            spriteRect_.x = spriteRect_.y = 0.f;
            spriteRect_.w = texture.getWidth();
            spriteRect_.h = texture.getHeight();
            updateRect();
        }
        void setSpriteRect(const SDL_FRect& rect)
        {
            spriteRect_ = rect;
        }

        #pragma endregion

        const SDL_FRect& getSpriteRect()
        {
            return spriteRect_;
        }

    private:

        const Texture* texturePtr_;

        SDL_FRect spriteRect_{};

        SDL_FPoint pivot_{};

    private:

        void draw(SDL_Renderer* renderer) const override
        {
            if(!texturePtr_)
                return;
            if(modifiedDrawable())
                SDL_RenderTextureRotated(renderer, texturePtr_->getRawTexture(), &spriteRect_, &distRect_, angle_, &pivot_,SDL_FLIP_NONE);
            else
                SDL_RenderTexture(renderer, texturePtr_->getRawTexture(), &spriteRect_, &distRect_);
        }

        void updateRect()
        {
            distRect_.w = texturePtr_->getWidth() * scale_;
            distRect_.h = texturePtr_->getHeight() * scale_;
            pivot_.x = distRect_.w / 2.f;
            pivot_.y = distRect_.h / 2.f;
        }


    };

} // sdlwrapper

#endif //KNIGHTS_AND_CASTLES_SPRITE_HPP
