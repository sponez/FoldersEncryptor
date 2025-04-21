#pragma once

#include <functional>
#include <string>
#include <vector>

#include <imgui.h>

#include "../window/abstract/GuiWindow.hpp"
#include "../sdl/SdlController.h"

namespace fe {
    class GuiUtils {
        public:
            static bool centredButton(const char* label) {
                float buttonWidth =
                    GuiWindow::properties.getPropertyValue<float>(GuiWindow::GuiWindowProperties::BUTTON_WIDTH_KEY)
                    * SdlController::properties.scale.first;

                float buttonHeight =
                    GuiWindow::properties.getPropertyValue<float>(GuiWindow::GuiWindowProperties::BUTTON_HEIGHT_KEY)
                    * SdlController::properties.scale.second;

                float windowWidth = ImGui::GetWindowSize().x;

                ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
                
                return ImGui::Button(label, ImVec2(buttonWidth , buttonHeight));
            }

            static void centeredButtonGroup(const std::vector<std::pair<std::string, std::function<void()>>>& actions) {
                float buttonHeight =
                    GuiWindow::properties.getPropertyValue<float>(GuiWindow::GuiWindowProperties::BUTTON_HEIGHT_KEY)
                    * SdlController::properties.scale.second;

                float spacing = ImGui::GetStyle().ItemSpacing.y * SdlController::properties.scale.second;
                float totalHeight = actions.size() * buttonHeight + (actions.size() - 1) * spacing;
                float startY = (ImGui::GetWindowSize().y - totalHeight) * 0.5f;
        
                ImGui::SetCursorPosY(startY);
        
                for (const auto& [label, callback] : actions) {
                    if (GuiUtils::centredButton(label.c_str())) {
                        callback();
                    }
                }
            }

        private:
            GuiUtils() {}
    };
}