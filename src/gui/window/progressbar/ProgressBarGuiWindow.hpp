#pragma once

#include <string>
#include <map>

#include <imgui.h>

#include "ProgressBarWindowAction.hpp"
#include "../GuiUtils.hpp"
#include "../abstract/GuiWindow.hpp"
#include "../../sdl/SdlController.h"
#include "../../../application/ApplicationRegistry.hpp"

namespace fe {
    class ProgressBarGuiWindow: public GuiWindow {
        private:
            ProgressBarGuiWindow() = default;
            ~ProgressBarGuiWindow() = default;

        public:
            ProgressBarWindowAction action = ProgressBarWindowAction::NONE;
            static ProgressBarGuiWindow& getInstance() {
                static ProgressBarGuiWindow instance;
                return instance;
            }

            std::size_t sizeToProcess = 0;

        protected:
            void draw() override {
                if (ApplicationRegistry::pull<bool>(ApplicationRegistry::Key::RUNNING)) {
                    float progress = *ApplicationRegistry::pull<float>(ApplicationRegistry::Key::PROCESSED) / sizeToProcess;
                    progress = std::min(progress, 1.0f);
                    ImGui::ProgressBar(progress, ImVec2(-1, 0));
                    ImGui::Text("%.1f%%", progress * 100.0f);
                } else {
                    sizeToProcess = 0;
                    action = ProgressBarWindowAction::DONE;
                }
            }
    };
}