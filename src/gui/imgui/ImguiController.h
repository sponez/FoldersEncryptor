#pragma once

#include <string>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

#include "../sdl/SdlController.h"
#include "../../utils/properties/Properties.hpp"
#include "../../utils/string/StringUtils.hpp"

namespace fe {
    class ImguiController {
        public:
            inline static const std::u8string GLSL_VERSION = u8"#version 130";

            static void init(SDL_Window* window, SDL_GLContext* glContext);
            static void destroy();
    };
}