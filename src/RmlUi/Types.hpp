#pragma once

#include <memory>

#include <RmlUi/Core.h>

#include <RmlUi_Platform_SDL.h>
#include <RmlUi_Renderer_SDL.h>

#include "FileInterface.hpp"

namespace rmluiwrapper
{
    
    using RenderInterfaceUPtr = std::unique_ptr<RenderInterface_SDL>;
    using SystemInterfaceUPtr = std::unique_ptr<SystemInterface_SDL>;
    using FileInterfaceUptr = std::unique_ptr<FileInterface>;


} // namespace rmluiwrapper
