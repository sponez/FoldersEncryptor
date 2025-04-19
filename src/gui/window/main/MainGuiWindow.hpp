#pragma once

#include <string>

#include <imgui.h>

#include "../abstract/GuiWindow.hpp"
#include "MainWindowAction.hpp"

namespace fe {
    class MainGuiWindow: public GuiWindow {
        private:
            inline static const std::string PROFILE_BUTTON_NAME = "Profile";
            inline static const std::string EXIT_BUTTON_NAME = "Exit";

            MainGuiWindow() {}

        public:
            static MainGuiWindow instance;
            MainWindowAction action = MainWindowAction::NONE;

            void draw() override {
                if (ImGui::Button(PROFILE_BUTTON_NAME.c_str())) {
                    action = MainWindowAction::PROFILE;
                }

                if (ImGui::Button(EXIT_BUTTON_NAME.c_str())) {
                    action = MainWindowAction::EXIT;
                }
            }
    };
}