#ifndef KNIGHTS_AND_CASTLES_TEXTURE_HPP
#define KNIGHTS_AND_CASTLES_TEXTURE_HPP

#include <string>
#include <unordered_map>
#include <memory>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "AdditionalSyntaxis.hpp"

namespace sdlwrapper
{
    class Texture
    {
    public:

        friend class RenderWindow;

    public:

        #pragma region init

        Texture() = default;
        Texture(Texture&& other) = default;
        Texture& operator=(Texture&& other) = default;
        
        Texture(const Texture& other) = delete;
        Texture& operator=(const Texture& other) = delete;

        Texture copy() const
        {
            SDL_Renderer* renderer = renderer_.lock().get();
            if(!renderer)
                return {};
            Texture res;
            res.renderer_ = renderer_;
            // Создаём пустую текстуру того же размера
            SDL_Texture* newTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, size_.x, size_.y);
            if(!newTex)
                return res;
            // Записываем оригинальную текстуру в новую
            SDL_SetRenderTarget(renderer, newTex);
            SDL_RenderTexture(renderer, texture_.get(), nullptr, nullptr);
            SDL_SetRenderTarget(renderer, nullptr);
            
            res.size_ = size_;
            res.texture_ = TextureUPtr{newTex};
            return res;
        }   

        #pragma endregion

        bool fillingWithColor(const SDL_FPoint size, const SDL_Color& color)
        {
            return fillingWithColor(size.x, size.y, color);
        }
        bool fillingWithColor(const float width, const float height, const SDL_Color& color)
        {
            texture_.reset();
            SDL_Renderer* renderer = renderer_.lock().get();
            if(!renderer)
                return false;
            SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);
            if(!texture)
                return false;
            SDL_SetRenderTarget(renderer, texture);  // Переключение на текстуру как render target
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); // Красный фон
            SDL_RenderClear(renderer);
            SDL_SetRenderTarget(renderer, nullptr); // Возврат на основной буфер
            texture_ = TextureUPtr{texture};
            size_.x = width;
            size_.y = height;
            return static_cast<bool>(texture_);
        }

        bool loadImage(const std::string& fileName)
        {
            texture_.reset();
            SDL_Renderer* renderer = renderer_.lock().get();
            if(!renderer)
                return false;
            auto imgSurface = IMG_Load(fileName.c_str());
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, imgSurface);
            SDL_DestroySurface(imgSurface);

            if(!tex)
                return false;

            texture_ = TextureUPtr{tex};
            calcSize();
            return static_cast<bool>(texture_);
        }

        bool empty() const
        {
            return !static_cast<bool>(texture_);
        }

        const float getWidth() const
        {
            return size_.x;
        }
        const float getHeight() const
        {
            return size_.y;
        }
        const SDL_FPoint& getSize() const
        {
            return size_;
        }
        
        SDL_Texture* getRawTexture() const
        {
            return texture_.get();
        }

    private:

        RendererWPtr renderer_ = {};

        TextureUPtr texture_ = nullptr;
        SDL_FPoint size_ = {};

    private:

        void calcSize()
        {
            if (!texture_)
            {
                size_ = {};
                return;
            }
            auto messageTexProps = SDL_GetTextureProperties(texture_.get());
            size_.x = float(SDL_GetNumberProperty(messageTexProps, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0));
            size_.y = float(SDL_GetNumberProperty(messageTexProps, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0));
        }

    };
    
} // sdlwrapper

#endif //KNIGHTS_AND_CASTLES_TEXTURE_HPP
