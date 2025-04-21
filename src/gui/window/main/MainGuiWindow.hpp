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
            inline static const std::string PROFILE_BUTTON_NAME = "Profile";
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
                GuiUtils::centeredButtonGroup(
                    {
                        { PROFILE_BUTTON_NAME, [](){ MainGuiWindow::getInstance().action = MainWindowAction::PROFILE; } },
                        { EXIT_BUTTON_NAME, [](){ MainGuiWindow::getInstance().action = MainWindowAction::EXIT; } }
                    }
                );
            }
    };
}