#pragma once

#include <functional>
#include <string>
#include <vector>

#include <imgui.h>

#include "../window/abstract/GuiWindow.hpp"
#include "../sdl/SdlController.h"
#include "../../utils/string/StringUtils.hpp"

namespace fe {
    class GuiUtils {
        public:
            struct Item {
                public:
                    std::u8string label;

                    Item(const std::u8string& label): label(label) {}

                    virtual float getWidth() = 0;
                    virtual float getHeight() = 0;
                    virtual void draw() = 0;
            };

            struct Button: public Item {
                public:
                    ImVec2 standartSize = GuiWindow::properties.getPropertyValue<ImVec2>(GuiWindow::GuiWindowProperties::BUTTON_SIZE_KEY);
                    std::function<void()> function;

                    Button(const std::u8string& label, const std::function<void()>& function): Item{label}, function(function) {}

                    float getWidth() override {
                        return standartSize.x * SdlController::properties.scale.first;
                    }

                    float getHeight() override {
                        return standartSize.y * SdlController::properties.scale.second;
                    }

                    void draw() override {
                        float buttonWidth = standartSize.x * SdlController::properties.scale.first;
                        float buttonHeight = standartSize.y * SdlController::properties.scale.second;
                        float windowWidth = ImGui::GetWindowSize().x;
        
                        ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
                        if (ImGui::Button(StringUtils::string(label).c_str(), ImVec2(buttonWidth , buttonHeight))) {
                            function();
                        }
                    }
            };

            struct InputText: public Item {
                public:
                    char* buffer;
                    const std::size_t& bufferSize;

                    InputText(const std::u8string& label, char* buffer, const std::size_t& bufferSize)
                        : Item{label}, buffer(buffer), bufferSize(bufferSize) {}

                    float getWidth() override {
                        return
                            GuiWindow::properties.getPropertyValue<float>(GuiWindow::GuiWindowProperties::INPUT_WIDTH_KEY)
                            * SdlController::properties.scale.first;
                    }

                    float getHeight() override {
                        return
                            ImGui::CalcTextSize(buffer).y
                            + 2.0f * ImGui::GetStyle().FramePadding.y;
                    }

                    void draw() override {
                        float inputWidth =
                        GuiWindow::properties.getPropertyValue<float>(GuiWindow::GuiWindowProperties::INPUT_WIDTH_KEY)
                        * SdlController::properties.scale.first;
                
                        float windowWidth = ImGui::GetWindowSize().x;
                
                        ImGui::SetCursorPosX((windowWidth - inputWidth) * 0.5f);
                        ImGui::PushItemWidth(inputWidth);
                        ImGui::InputText(StringUtils::string(label).c_str(), buffer, bufferSize);
                        ImGui::PopItemWidth();
                    }
            };

            struct Text: public Item {
                public:
                    std::u8string text;

                    Text(const std::u8string& label, const std::u8string& text)
                        : Item{label}, text(text) {}

                    float getWidth() override {
                        return ImGui::CalcTextSize(StringUtils::string(text).c_str()).x;
                    }

                    float getHeight() override {
                        return ImGui::CalcTextSize(StringUtils::string(text).c_str()).y;
                    }

                    void draw() override {
                        float textWidth = ImGui::CalcTextSize(StringUtils::string(text).c_str()).x;
                        float windowWidth = ImGui::GetWindowSize().x;
                        
                        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
                        ImGui::TextUnformatted(StringUtils::string(text).c_str());
                    }
            };

            static void itemGroup(const std::vector<std::shared_ptr<Item>>& items) {
                float totalHeight = 0.0f;
                for (const auto& item : items) {
                    totalHeight += item->getHeight();
                }
            
                totalHeight += ImGui::GetStyle().ItemSpacing.y * (items.size() - 1);

                float startY = (ImGui::GetWindowSize().y - totalHeight) * 0.5f;
                ImGui::SetCursorPosY(startY);
            
                for (const auto& item : items) {
                    item->draw();
                }
            }

            static void renderBackButton(const std::function<void()>& onClick) {
                float padding = ImGui::GetStyle().FramePadding.y * SdlController::properties.scale.second;
                ImGui::SetCursorPos(ImVec2(padding, padding));
            
                if (ImGui::Button("↩")) {
                    onClick();
                }
            }

        private:
            GuiUtils() {}
    };
}