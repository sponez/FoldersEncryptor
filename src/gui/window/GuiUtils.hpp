#pragma once

#include <imgui.h>

#include "../sdl/SdlController.h"

namespace fe {
    class GuiUtils {
        public:
            inline static float BUTTON_SIZE_WIDTH = 100.0f;
            inline static float BUTTON_SIZE_HEIGHT = 30.0f;

            static bool centredButton(const char* label, const ImVec2& size_arg = ImVec2(BUTTON_SIZE_WIDTH, BUTTON_SIZE_HEIGHT)) {
                float windowWidth = ImGui::GetWindowSize().x;
                ImGui::SetCursorPosX((windowWidth - size_arg.x) * 0.5f);
                
                return ImGui::Button(label, ImVec2(size_arg.x * SdlController::properties.scale.first, size_arg.y * SdlController::properties.scale.second));
            }

        private:
            GuiUtils() {}
    };
}