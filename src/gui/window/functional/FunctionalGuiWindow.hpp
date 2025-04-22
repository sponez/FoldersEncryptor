#pragma once

#include <array>
#include <string>

#include <imgui.h>

#include "FunctionalWindowAction.hpp"
#include "../abstract/GuiWindow.hpp"
#include "../GuiUtils.hpp"

namespace fe {
    class FunctionalGuiWindow: public GuiWindow {
        private:
            inline static const std::string ENRYPT_BUTTON_NAME = "Enctypt";
            inline static const std::string DERYPT_BUTTON_NAME = "Decrypt";
            inline static const std::string TEMPORARY_OPEN_BUTTON_NAME = "Temporary open";
            inline static const std::string PROPERTIES_BUTTON_NAME = "Properties";

            FunctionalGuiWindow() = default;
            ~FunctionalGuiWindow() = default;
        
        public:
            FunctionalWindowAction action = FunctionalWindowAction::NONE;
            static FunctionalGuiWindow& getInstance() {
                static FunctionalGuiWindow instance;
                return instance;
            }

            protected:
            void draw() override {
                GuiUtils::renderBackButton(
                    []() { FunctionalGuiWindow::getInstance().action = FunctionalWindowAction::BACK; }
                );

                GuiUtils::itemGroup(
                    {
                        std::make_shared<GuiUtils::Button>(ENRYPT_BUTTON_NAME, []() { FunctionalGuiWindow::getInstance().action = FunctionalWindowAction::ENCRYPT; }),
                        std::make_shared<GuiUtils::Button>(DERYPT_BUTTON_NAME, []() { FunctionalGuiWindow::getInstance().action = FunctionalWindowAction::DECTYPT; }),
                        std::make_shared<GuiUtils::Button>(TEMPORARY_OPEN_BUTTON_NAME, []() { FunctionalGuiWindow::getInstance().action = FunctionalWindowAction::TEMPORARY_OPEN; }),
                        std::make_shared<GuiUtils::Button>(PROPERTIES_BUTTON_NAME, []() { FunctionalGuiWindow::getInstance().action = FunctionalWindowAction::PROPERTIES; })
                    }
                );
            }
    };
}