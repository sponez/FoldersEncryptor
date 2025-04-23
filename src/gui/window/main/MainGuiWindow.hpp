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
            inline static const std::u8string LOGIN_BUTTON_NAME = u8"Login";
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
                        std::make_shared<GuiUtils::Button>(LOGIN_BUTTON_NAME, [](){ MainGuiWindow::getInstance().action = MainWindowAction::LOGIN; }),
                        std::make_shared<GuiUtils::Button>(EXIT_BUTTON_NAME, [](){ MainGuiWindow::getInstance().action = MainWindowAction::EXIT; })
                    }
                );
            }
    };
}