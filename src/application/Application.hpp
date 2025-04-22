#pragma once

#include <string>

#include "../gui/GuiGlobalController.hpp"
#include "../utils/properties/Properties.hpp"

namespace fe {
    class Application {
        public:
            class ApplicationProperties: public Properties {
                public:
                    inline static const std::u8string USER_KEY = u8"user";
                    inline static const std::u8string PASSWORD_KEY = u8"password";
            };
        
            inline static Application::ApplicationProperties properties;

            static void start() {
                GuiGlobalController::startGui();
            }
    };
}