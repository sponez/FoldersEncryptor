#pragma once

#include <array>
#include <optional>
#include <string>

#include <imgui.h>

#include "PropertiesWindowAction.hpp"
#include "../abstract/GuiWindow.hpp"
#include "../GuiUtils.hpp"
#include "../../../utils/storage/StorageChecker.hpp"

namespace fe {
    class PropertiesGuiWindow: public GuiWindow {
        private:
            inline static const std::string SYSTEM_TAB = "System";
            inline static const std::string APPEARANCE_TAB = "Appearance";

            inline static const std::string APPLY_BUTTON_NAME = "Apply";
            inline static const std::string CANCEL_BUTTON_NAME = "Cancel";
            inline static const std::string RESTORE_DEFAULTS_BUTTON_NAME = "Restore defaults";

            PropertiesGuiWindow() = default;
            ~PropertiesGuiWindow() = default;

        public:
            PropertiesWindowAction action = PropertiesWindowAction::NONE;
            static PropertiesGuiWindow& getInstance() {
                static PropertiesGuiWindow instance;
                return instance;
            }

            inline static std::optional<bool> usePerFilePassword = std::nullopt;
            inline static std::optional<bool> bindToStorageId = std::nullopt;
        
        protected:
            void draw() override {
                GuiUtils::renderBackButton(
                    []() { PropertiesGuiWindow::getInstance().action = PropertiesWindowAction::BACK; }
                );

                if (ImGui::BeginTabBar("SettingsTabs")) {
                    if (ImGui::BeginTabItem(SYSTEM_TAB.c_str())) {
                        systemTab();
                        ImGui::EndTabItem();
                    }
        
                    if (ImGui::BeginTabItem(APPEARANCE_TAB.c_str())) {
                        appearanceTab();
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }

                GuiUtils::Button(APPLY_BUTTON_NAME, []() { PropertiesGuiWindow::getInstance().action = PropertiesWindowAction::APPLY; }).draw();
            }

        private:
            void systemTab() {
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 4));
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 3));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

                ImGui::Checkbox("Use individual password for each file", &usePerFilePassword.value());
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Allows adding a unique password for every file.\nIncreases security, but makes recovery harder.");
                }
            
                ImGui::BeginDisabled(!isRunningFromRemovableMedia());
                ImGui::Checkbox("Bind encryption to USB ID", &bindToStorageId.value());
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Makes files decryptable only on specific hardware.\nAvalable only on USB drives.");
                }
                ImGui::EndDisabled();

                ImGui::PopStyleVar(3);
            }

            void appearanceTab() {
                
            }
    };
}