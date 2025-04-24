#pragma once

#include <string>
#include <map>

#include <imgui.h>

#include "ErrorWindowAction.hpp"
#include "../GuiUtils.hpp"
#include "../abstract/GuiWindow.hpp"
#include "../../sdl/SdlController.h"
#include "../../../application/ApplicationRegistry.hpp"

namespace fe {
    class ErrorGuiWindow: public GuiWindow {
        private:
            inline static const std::u8string OK_BUTTON_NAME = u8"OK";

            ErrorGuiWindow() = default;
            ~ErrorGuiWindow() = default;

        public:
            ErrorWindowAction action = ErrorWindowAction::NONE;
            static ErrorGuiWindow& getInstance() {
                static ErrorGuiWindow instance;
                return instance;
            }

        protected:
            void draw() override {
                std::string errorMessage = *ApplicationRegistry::pull<std::string>(ApplicationRegistry::Key::ENCRYPTION_ERROR);

                ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_Appearing);
                ImGui::OpenPopup("Error");
                
                if (ImGui::BeginPopupModal("Error", nullptr)) {
                    ImGui::TextWrapped("%s", errorMessage.c_str());
                    if (ImGui::Button("OK")) {
                        ImGui::CloseCurrentPopup();
                        action = ErrorWindowAction::OK;
                    }
                    ImGui::EndPopup();
                }
            }
    };
}