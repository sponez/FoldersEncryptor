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
            inline static const std::string LOGIN_BUTTON_NAME = "Login";
            inline static const std::string EXIT_BUTTON_NAME = "Exit";

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