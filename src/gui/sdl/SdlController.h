#pragma once

#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include <SDL.h>
#include <SDL_opengl.h>

#include <rapidjson/document.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include "../../utils/properties/Properties.hpp"
#include "../../utils/properties/PropertiesLoader.hpp"
#include "../../utils/properties/PropertiesFileBuilder.hpp"
#include "../../utils/string/StringUtils.hpp"

namespace fe {
    class SdlController {
        public:
            class SdlProperties: public Properties {
                public:
                    inline static const std::u8string FILE = u8"sdl/.properties";
                    inline static const std::u8string APPLICATION_NAME = u8"Folders Encryptor";

                    inline static const std::u8string MONITOR_ID_KEY = u8"monitorId";
                    inline static const int DEFAULT_MONITOR_ID_VALUE = 0;

                    inline static const std::u8string WINDOW_POSITION_X_KEY = u8"windowPostitionX";
                    inline static const int DEFAULT_WINDOW_POSITION_X_VALUE = SDL_WINDOWPOS_CENTERED;

                    inline static const std::u8string WINDOW_POSITION_Y_KEY = u8"windowPostitionY";
                    inline static const int DEFAULT_WINDOW_POSITION_Y_VALUE = SDL_WINDOWPOS_CENTERED;

                    inline static const std::u8string WINDOW_WIDTH_KEY = u8"windowWidth";
                    inline static const int DEFAULT_WINDOW_WIDTH_VALUE = 600;

                    inline static const std::u8string WINDOW_HEIGHT_KEY = u8"windowHeight";
                    inline static const int DEFAULT_WINDOW_HEIGHT_VALUE = 300;

                    inline static const int VIEWPORT_X = 0;
                    inline static const int VIEWPORT_Y = 0;
                    inline static const float CLEAR_COLOR_R = 0.1f;
                    inline static const float CLEAR_COLOR_G = 0.1f;
                    inline static const float CLEAR_COLOR_B = 0.1f;
                    inline static const float CLEAR_COLOR_A = 1.0f;

                    inline static std::pair<float, float> scale;
            };

        private:
            inline static SDL_Window* window = nullptr;
            inline static SDL_GLContext glContext = nullptr;

        public:
            inline static SdlController::SdlProperties properties;

            static void init();
            static void destroy();

            static void render();
            
            static void saveProperties();
            static void loadProperties();
            static void updateProperties();
        
            static void findScale();

        private:
            static void createWindow();
            static void createGlContext();
            static void enableVSync();
    };
}