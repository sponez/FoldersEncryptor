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
            inline static const std::u8string SYSTEM_TAB = u8"System";
            inline static const std::u8string APPEARANCE_TAB = u8"Appearance";

            inline static const std::u8string APPLY_BUTTON_NAME = u8"Apply";
            inline static const std::u8string CANCEL_BUTTON_NAME = u8"Cancel";
            inline static const std::u8string RESTORE_DEFAULTS_BUTTON_NAME = u8"Restore defaults";

            PropertiesGuiWindow() = default;
            ~PropertiesGuiWindow() = default;

        public:
            PropertiesWindowAction action = PropertiesWindowAction::NONE;
            static PropertiesGuiWindow& getInstance() {
                static PropertiesGuiWindow instance;
                return instance;
            }

            inline static std::optional<bool> useAuthorization = std::nullopt;
            inline static std::optional<bool> usePerFilePassword = std::nullopt;
            inline static std::optional<bool> bindToStorageId = std::nullopt;
        
        protected:
            void draw() override {
                GuiUtils::renderBackButton(
                    []() { PropertiesGuiWindow::getInstance().action = PropertiesWindowAction::BACK; }
                );

                if (ImGui::BeginTabBar("SettingsTabs")) {
                    if (ImGui::BeginTabItem(StringUtils::string(SYSTEM_TAB).c_str())) {
                        systemTab();
                        ImGui::EndTabItem();
                    }
        
                    if (ImGui::BeginTabItem(StringUtils::string(APPEARANCE_TAB).c_str())) {
                        appearanceTab();
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }

                GuiUtils::Button(
                    APPLY_BUTTON_NAME,
                    [&]() { action = PropertiesWindowAction::APPLY; }
                ).draw();
            }

        private:
            void systemTab() {
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 4));
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 3));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

                ImGui::Checkbox("Use authorization", &useAuthorization.value());
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Use username and password to protect your files.");
                }

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