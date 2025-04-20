#pragma once

#include <imgui.h>

namespace fe {
    class GuiWindow {
        public:
            void setAndDraw() {
                ImGui::SetNextWindowPos(ImVec2(0, 0));
                ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
                ImGui::Begin(
                    "Window", nullptr,
                    ImGuiWindowFlags_NoDecoration |
                    ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoSavedSettings |
                    ImGuiWindowFlags_NoTitleBar
                );
                ImGui::SetCursorPos(ImVec2(0, 0));

                draw();

                ImGui::End();
            }

        protected:
            virtual void draw() = 0;
    };
}