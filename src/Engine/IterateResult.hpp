#pragma once

#include <string>
#include <map>
#include <variant>
#include <memory>

#include "Types.hpp"

namespace engine
{
    
    enum class IterateResultType : unsigned char
    {
        none = 0,   //std::monostate
        next,       //std::monostate
        close,      //std::monostate
        newScene,   //SceneSettings
        addScene    //SceneSettings
    };

    using ResultInfo = std::variant
    <
        std::monostate, //ничего
        IDType          //id новой сцены
    >;

    class IterateResult
    {
    public:
        IterateResult() : type_{IterateResultType::none}, info_{std::monostate{}} {}

        #pragma region fabrick

        static IterateResult next()
        {
            return IterateResult(IterateResultType::next, std::monostate{});
        }
        static IterateResult close()
        {
            return IterateResult(IterateResultType::close, std::monostate{});
        }
        static IterateResult newScene(const IDType newSceneId)
        {
            return IterateResult(IterateResultType::newScene, newSceneId);
        }
        static IterateResult addScene(const IDType additionalSceneId)
        {
            return IterateResult(IterateResultType::addScene, additionalSceneId);
        }

        #pragma endregion

        const IterateResultType getType() const
        {
            return type_;
        }

        IDType getNextSceneId() const
        {
            if (type_ != IterateResultType::newScene && type_ != IterateResultType::addScene)
                return 0;
            const IDType* ptr = std::get_if<IDType>(&info_);
            return ptr ? *ptr : 0;
        }

    private:

        IterateResultType type_;
        ResultInfo info_;

    private:
        IterateResult(IterateResultType type, ResultInfo info) :
            type_{type}, info_{std::move(info)}
        {} 
        

    };

} // namespace engine
