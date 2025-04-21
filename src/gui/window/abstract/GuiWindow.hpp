#pragma once

#include <string>

#include <imgui.h>

#include "../../../utils/properties/Properties.hpp"
#include "../../../utils/properties/PropertiesLoader.hpp"
#include "../../../utils/properties/PropertiesFileBuilder.hpp"
#include "../../../utils/string/StringUtils.hpp"

namespace fe {
    class GuiWindow {
        public:
            class GuiWindowProperties: public Properties {
                public:
                    inline static const std::u8string FILE = u8"imgui/window.properties";

                    inline static const std::u8string BUTTON_WIDTH_KEY = u8"duttonWidth";
                    inline static const float DEFAULT_BUTTON_WIDTH_VALUE = 160.0f;

                    inline static const std::u8string BUTTON_HEIGHT_KEY = u8"duttonHeight";
                    inline static const float DEFAULT_BUTTON_HEIGHT_VALUE = 36.0f;

                    inline static const std::u8string SPASE_X_KEY = u8"spaceX";
                    inline static const float DEFAULT_SPASE_X_VALUE = 16.0f;

                    inline static const std::u8string SPASE_Y_KEY = u8"spaceY";
                    inline static const float DEFAULT_SPASE_Y_VALUE = 16.0f;
            };

            inline static GuiWindow::GuiWindowProperties properties;

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

            static void saveProperties() {
                std::string jsonFile =
                PropertiesFileBuilder(&properties)
                    .save<float>(GuiWindow::GuiWindowProperties::BUTTON_WIDTH_KEY)
                    ->save<float>(GuiWindow::GuiWindowProperties::BUTTON_HEIGHT_KEY)
                    ->save<float>(GuiWindow::GuiWindowProperties::SPASE_X_KEY)
                    ->save<float>(GuiWindow::GuiWindowProperties::SPASE_Y_KEY)
                    ->build();
    
                std::filesystem::path savePath = StringUtils::path(GuiWindow::GuiWindowProperties::FILE);
                std::filesystem::create_directory(savePath.parent_path());
                std::ofstream out(savePath);
                out << jsonFile;
                out.close();
            }

            static void loadProperties() {
                PropertiesLoader loader(GuiWindow::GuiWindowProperties::FILE, &properties);

                loader.loadOrDefault<float>(
                    GuiWindow::GuiWindowProperties::BUTTON_WIDTH_KEY,
                    GuiWindow::GuiWindowProperties::DEFAULT_BUTTON_WIDTH_VALUE
                )->loadOrDefault<float>(
                    GuiWindow::GuiWindowProperties::BUTTON_HEIGHT_KEY,
                    GuiWindow::GuiWindowProperties::DEFAULT_BUTTON_HEIGHT_VALUE
                )->loadOrDefault<float>(
                    GuiWindow::GuiWindowProperties::SPASE_X_KEY,
                    GuiWindow::GuiWindowProperties::DEFAULT_SPASE_X_VALUE
                )->loadOrDefault<float>(
                    GuiWindow::GuiWindowProperties::SPASE_Y_KEY,
                    GuiWindow::GuiWindowProperties::DEFAULT_SPASE_Y_VALUE
                );
            }

        protected:
            virtual void draw() = 0;
    };
}