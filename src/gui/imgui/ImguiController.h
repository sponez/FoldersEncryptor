#pragma once

#include <string>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

#include "ImguiTheme.h"
#include "../sdl/SdlController.h"
#include "../../utils/properties/Properties.hpp"
#include "../../utils/string/StringUtils.hpp"

namespace fe {
    class ImguiController {
        public:
            class ImguiProperties: public Properties {
                public:
                    inline static const std::u8string FILE = u8"imgui/properties";
                    inline static const std::u8string GLSL_VERSION = u8"#version 130";

                    inline static const std::u8string FONT_KEY = u8"font";
                    inline static const std::string DEFAULT_FONT_VALUE = "";

                    inline static const std::u8string FONT_SIZE_KEY = u8"fontSize";
                    inline static const float DEFAULT_FONT_SIZE_VALUE = 16.0f;

                    inline static const std::u8string THEME_KEY = u8"theme";
                    inline static const ImguiTheme DEFAULT_THEME_VALUE = ImguiTheme::DARK;
            };

        public:
            inline static ImguiController::ImguiProperties properties;

            static void init(SDL_Window* window, SDL_GLContext* glContext);
            static void destroy();

            static void saveProperties();
            static void loadProperties();

        private:
            static void setFont();
            static void setTheme();
    };
}