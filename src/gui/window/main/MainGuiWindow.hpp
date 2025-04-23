#pragma once

#include <string>
#include <map>

#include <imgui.h>

#include "MainWindowAction.hpp"
#include "../GuiUtils.hpp"
#include "../abstract/GuiWindow.hpp"
#include "../../sdl/SdlController.h"

namespace fe {
    class MainGuiWindow: public GuiWindow {
        private:
            inline static const std::u8string ENTER_BUTTON_NAME = u8"Enter";
            inline static const std::u8string PROPERTIES_BUTTON_NAME = u8"Properties";
            inline static const std::u8string EXIT_BUTTON_NAME = u8"Exit";

            MainGuiWindow() = default;
            ~MainGuiWindow() = default;

        public:
            MainWindowAction action = MainWindowAction::NONE;
            static MainGuiWindow& getInstance() {
                static MainGuiWindow instance;
                return instance;
            }

        protected:
            void draw() override {
                GuiUtils::itemGroup(
                    {
                        std::make_shared<GuiUtils::Button>(
                            ENTER_BUTTON_NAME,
                            [&](){
                                if (isUsingAuthorization()) {
                                    action = MainWindowAction::LOGIN;
                                } else {
                                    action = MainWindowAction::FUNCTIONAL;
                                }
                            }
                        ),
                        std::make_shared<GuiUtils::Button>(
                            PROPERTIES_BUTTON_NAME,
                            [&]() { action = MainWindowAction::PROPERTIES; }
                        ),
                        std::make_shared<GuiUtils::Button>(
                            EXIT_BUTTON_NAME,
                            [&](){ action = MainWindowAction::EXIT; }
                        )
                    }
                );
            }
        
        private:
            bool isUsingAuthorization();
    };
}