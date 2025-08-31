set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE INTERNAL "")

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  if(NOT CMAKE_GENERATOR STREQUAL "Ninja")
    add_definitions(/MP)
  endif()
endif()

# ---------- СОЗДАНИЕ ТАРГЕТА ПРИЛОЖЕНИЯ ----------

add_executable(${EXECUTABLE_NAME} ${SRC_DIR}/main.cpp)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
  ${PROJECT_SOURCE_DIR}
)

if(APPLE)
  target_sources(${EXECUTABLE_NAME} PRIVATE "src/logo.png")
endif()

# ---------- SDL3 ----------

# Позволяет переключиться на заранее собранные бинарники SDL3
option(USE_SDL_BINARIES "Use precompiled SDL3" OFF)
set(SDL3_BINARIES_PATH "../vendor/SDL3-3.x.x" CACHE PATH "Path to prebuilt SDL3 (contains cmake/SDL3Config.cmake)")
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON  CACHE BOOL "" FORCE)

add_subdirectory(${EXTERNAL_DIR}/SDL "${THIRD_PARTY_BUILD_DIR}/SDL" EXCLUDE_FROM_ALL)
# ---------- SDL3_image ----------

# Аналогично: можно использовать бинарники, по умолчанию — исходники
option(USE_SDL_IMAGE_BINARIES "Use precompiled SDL3_image" OFF)
set(SDL_IMAGE_BINARIES_PATH "../vendor/SDL3_image-3.x.x" CACHE PATH "Path to prebuilt SDL3_image (cmake config)")
set(SDLIMAGE_VENDORED ON  CACHE BOOL "" FORCE)
set(SDLIMAGE_SHARED  OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_STATIC  ON  CACHE BOOL "" FORCE)
set(SDLIMAGE_TIFF   OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_AVIF   OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_JPEG   ON CACHE BOOL "" FORCE)
set(SDLIMAGE_WEBP   OFF CACHE BOOL "" FORCE)

add_subdirectory(${EXTERNAL_DIR}/SDL_image "${THIRD_PARTY_BUILD_DIR}/SDL_image" EXCLUDE_FROM_ALL)
# ---------- FreeType (СТАТИК, без Brotli) ----------

# Вырезаем внешние зависимости, чтобы не появлялись сторонние DLL
set(FT_DISABLE_BROTLI   ON CACHE BOOL "" FORCE)  # ключевой момент: не будет brotli*.dll
set(FT_DISABLE_ZLIB     ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2    ON CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG      ON CACHE BOOL "" FORCE)
set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)

add_subdirectory(${EXTERNAL_DIR}/freetype "${THIRD_PARTY_BUILD_DIR}/freetype" EXCLUDE_FROM_ALL)
if(NOT TARGET Freetype::Freetype)
  add_library(Freetype::Freetype ALIAS freetype)
endif()
# ---------- SDL3_mixer (СТАТИК) ----------

set(SDLMIXER_SHARED   OFF CACHE BOOL "" FORCE)
set(SDLMIXER_STATIC   ON  CACHE BOOL "" FORCE)
set(SDLMIXER_VENDORED ON  CACHE BOOL "" FORCE)
set(SDLMIXER_MIDI_NATIVE OFF CACHE BOOL "" FORCE)
set(SDLMIXER_GME         OFF CACHE BOOL "" FORCE)
set(SDLMIXER_WAVPACK     OFF CACHE BOOL "" FORCE)
set(SDLMIXER_MOD         OFF CACHE BOOL "" FORCE)
set(SDLMIXER_OPUS        OFF CACHE BOOL "" FORCE)

add_subdirectory(${EXTERNAL_DIR}/SDL_mixer "${THIRD_PARTY_BUILD_DIR}/SDL_mixer" EXCLUDE_FROM_ALL)

# ---------- RmlUi (СТАТИК) ----------

# Большинство версий уважают глобальный BUILD_SHARED_LIBS=OFF; на всякий случай:
set(RMLUI_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(${EXTERNAL_DIR}/RmlUi "${THIRD_PARTY_BUILD_DIR}/RmlUi" EXCLUDE_FROM_ALL)

# Бэкенды RmlUi под SDL3
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${EXTERNAL_DIR}/RmlUi/Backends)
target_sources(${EXECUTABLE_NAME} PRIVATE
  ${EXTERNAL_DIR}/RmlUi/Backends/RmlUi_Platform_SDL.cpp
  ${EXTERNAL_DIR}/RmlUi/Backends/RmlUi_Renderer_SDL.cpp
)
target_compile_definitions(${EXECUTABLE_NAME} PRIVATE
  RMLUI_SDL_VERSION_MAJOR=3
  RMLUI_STATIC_LIB
)

# ---------- SDL3_gfx (исходники прямо в таргет) ----------

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${EXTERNAL_DIR}/SDL3_gfx)
target_sources(${EXECUTABLE_NAME} PRIVATE
  ${EXTERNAL_DIR}/SDL3_gfx/SDL3_gfx/SDL3_framerate.c
  ${EXTERNAL_DIR}/SDL3_gfx/SDL3_gfx/SDL3_gfxPrimitives.c
  ${EXTERNAL_DIR}/SDL3_gfx/SDL3_gfx/SDL3_imageFilter.c
  ${EXTERNAL_DIR}/SDL3_gfx/SDL3_gfx/SDL3_rotozoom.c
)

# ---------- ЛИНКОВКА ----------

# Выбираем статические таргеты, если доступны
if(TARGET SDL3::SDL3-static)
  set(SDL3_TARGET SDL3::SDL3-static)
else()
  set(SDL3_TARGET SDL3::SDL3)
endif()

if(TARGET SDL3_image::SDL3_image-static)
  set(SDL3_IMAGE_TARGET SDL3_image::SDL3_image-static)
else()
  set(SDL3_IMAGE_TARGET SDL3_image::SDL3_image)
endif()

if(TARGET SDL3_mixer::SDL3_mixer-static)
  set(SDL3_MIXER_TARGET SDL3_mixer::SDL3_mixer-static)
else()
  set(SDL3_MIXER_TARGET SDL3_mixer::SDL3_mixer)
endif()

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
  ${SDL3_MIXER_TARGET}
  ${SDL3_IMAGE_TARGET}
  ${SDL3_TARGET}
  RmlUi::RmlUi
  Freetype::Freetype
)

# ---------- ПЛАТФОРМЕННЫЕ ФИШКИ ----------

# Emscripten: ассеты
if(EMSCRIPTEN)
  file(GLOB ASSET_FILES ${ASSETS_DIR}/*)
  foreach(ASSET_FILE IN LISTS ASSET_FILES)
    get_filename_component(FILENAME ${ASSET_FILE} NAME)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE "--preload-file ${ASSETS_DIR}/${FILENAME}")
  endforeach()
endif()

# iOS ресурсы
if(IOS)
  set(RESOURCE_FILES "src/iosLaunchScreen.storyboard;src/logo.png")
endif()

# macOS бандл
set_target_properties(${EXECUTABLE_NAME} PROPERTIES
  MACOSX_BUNDLE TRUE
  MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/src/Info.plist.in"
  XCODE_GENERATE_SCHEME TRUE
  XCODE_ATTRIBUTE_BUNDLE_IDENTIFIER "com.asdrome.sdl3-template"
  XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.asdrome.sdl3-template"
  RESOURCE "${RESOURCE_FILES}"
)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "${EXECUTABLE_NAME}")

if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
  install(TARGETS ${EXECUTABLE_NAME}
    BUNDLE DESTINATION ./install COMPONENT Runtime
    RUNTIME DESTINATION ./install/bin COMPONENT Runtime
  )
  set(DEP_DIR "${CMAKE_BINARY_DIR}")
  install(CODE
    "include(BundleUtilities)
     fixup_bundle(\"$<TARGET_BUNDLE_DIR:${EXECUTABLE_NAME}>\" \"\" \"${DEP_DIR}\")"
  )
  set(CPACK_GENERATOR "DragNDrop")
  include(CPack)
endif()

if(NOT USE_CONSOLE)
  if(MSVC)
    target_link_options(${EXECUTABLE_NAME} PRIVATE "/SUBSYSTEM:WINDOWS")
  elseif(MINGW)
    target_link_options(${EXECUTABLE_NAME} PRIVATE "-mwindows")
  endif()
endif()