#pragma once

#include <string>

#include <imgui.h>

#include "ImguiTheme.h"
#include "../../../utils/properties/Properties.hpp"
#include "../../../utils/properties/PropertiesLoader.hpp"
#include "../../../utils/properties/PropertiesFileBuilder.hpp"
#include "../../../utils/string/StringUtils.hpp"

namespace fe {
    class GuiWindow {
        public:
            class GuiWindowProperties: public Properties {
                public:
                    inline static const std::u8string FILE = u8"imgui/.properties";

                    inline static const std::u8string BUTTON_WIDTH_KEY = u8"buttonWidth";
                    inline static const float DEFAULT_BUTTON_WIDTH_VALUE = 160.0f;

                    inline static const std::u8string BUTTON_HEIGHT_KEY = u8"buttonHeight";
                    inline static const float DEFAULT_BUTTON_HEIGHT_VALUE = 36.0f;

                    inline static const std::u8string SPASE_X_KEY = u8"spaceX";
                    inline static const float DEFAULT_SPASE_X_VALUE = 16.0f;

                    inline static const std::u8string SPASE_Y_KEY = u8"spaceY";
                    inline static const float DEFAULT_SPASE_Y_VALUE = 16.0f;

                    inline static const std::u8string BUTTON_HOVERED_COLOR_KEY = u8"buttonHoveredColor";
                    inline static const ImVec4 DEFAULT_BUTTON_HOVERED_COLOR_VALUE = ImVec4(0.25f, 0.45f, 0.75f, 1.0f);

                    inline static const std::u8string BUTTON_ACTIVE_COLOR_KEY = u8"buttonActiveColor";
                    inline static const ImVec4 DEFAULT_BUTTON_ACTIVE_COLOR_VALUE = ImVec4(0.15f, 0.35f, 0.55f, 1.0f);

                    inline static const std::u8string FRAME_ROUNDING_KEY = u8"frameRounding";
                    inline static const float DEFAULT_FRAME_ROUNDING_VALUE = 6.0f;

                    inline static const std::u8string FRAME_BORDER_SIZE_KEY = u8"frameBorderSize";
                    inline static const float DEFAULT_FRAME_BORDER_SIZE_VALUE = 1.0f;

                    inline static const std::u8string FRAME_BORDER_COLOR_KEY = u8"frameBorderColor";
                    inline static const ImVec4 DEFAULT_FRAME_BORDER_COLOR_VALUE = ImVec4(1, 1, 1, 0.2f);

                    inline static const std::u8string INPUT_WIDTH_KEY = u8"inputWidth";
                    inline static const float DEFAULT_INPUT_WIDTH_VALUE = 200.0f;

                    inline static const std::u8string FONT_KEY = u8"font";
                    inline static const std::string DEFAULT_FONT_VALUE = "";

                    inline static const std::u8string FONT_SIZE_KEY = u8"fontSize";
                    inline static const float DEFAULT_FONT_SIZE_VALUE = 16.0f;

                    inline static const std::u8string THEME_KEY = u8"theme";
                    inline static const ImguiTheme DEFAULT_THEME_VALUE = ImguiTheme::DARK;
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
                    ->save<ImVec4>(GuiWindow::GuiWindowProperties::BUTTON_HOVERED_COLOR_KEY)
                    ->save<ImVec4>(GuiWindow::GuiWindowProperties::BUTTON_ACTIVE_COLOR_KEY)
                    ->save<float>(GuiWindow::GuiWindowProperties::FRAME_ROUNDING_KEY)
                    ->save<float>(GuiWindow::GuiWindowProperties::FRAME_BORDER_SIZE_KEY)
                    ->save<ImVec4>(GuiWindow::GuiWindowProperties::FRAME_BORDER_COLOR_KEY)
                    ->save<float>(GuiWindow::GuiWindowProperties::INPUT_WIDTH_KEY)
                    ->save<std::string>(GuiWindow::GuiWindowProperties::FONT_KEY)
                    ->save<float>(GuiWindow::GuiWindowProperties::FONT_SIZE_KEY)
                    ->save<int>(GuiWindow::GuiWindowProperties::THEME_KEY)
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
                )->loadOrDefault<ImVec4>(
                    GuiWindow::GuiWindowProperties::BUTTON_HOVERED_COLOR_KEY,
                    GuiWindow::GuiWindowProperties::DEFAULT_BUTTON_HOVERED_COLOR_VALUE
                )->loadOrDefault<ImVec4>(
                    GuiWindow::GuiWindowProperties::BUTTON_ACTIVE_COLOR_KEY,
                    GuiWindow::GuiWindowProperties::DEFAULT_BUTTON_ACTIVE_COLOR_VALUE
                )->loadOrDefault<float>(
                    GuiWindow::GuiWindowProperties::FRAME_ROUNDING_KEY,
                    GuiWindow::GuiWindowProperties::DEFAULT_FRAME_ROUNDING_VALUE
                )->loadOrDefault<float>(
                    GuiWindow::GuiWindowProperties::FRAME_BORDER_SIZE_KEY,
                    GuiWindow::GuiWindowProperties::DEFAULT_FRAME_BORDER_SIZE_VALUE
                )->loadOrDefault<ImVec4>(
                    GuiWindow::GuiWindowProperties::FRAME_BORDER_COLOR_KEY,
                    GuiWindow::GuiWindowProperties::DEFAULT_FRAME_BORDER_COLOR_VALUE
                )->loadOrDefault<float>(
                    GuiWindow::GuiWindowProperties::INPUT_WIDTH_KEY,
                    GuiWindow::GuiWindowProperties::DEFAULT_INPUT_WIDTH_VALUE
                )->loadOrDefault<std::string>(
                    GuiWindow::GuiWindowProperties::FONT_KEY,
                    GuiWindow::GuiWindowProperties::DEFAULT_FONT_VALUE
                )->loadOrDefault<float>(
                    GuiWindow::GuiWindowProperties::FONT_SIZE_KEY,
                    GuiWindow::GuiWindowProperties::DEFAULT_FONT_SIZE_VALUE
                )->loadOrDefault<int>(
                    GuiWindow::GuiWindowProperties::THEME_KEY,
                    static_cast<int>(GuiWindow::GuiWindowProperties::DEFAULT_THEME_VALUE)
                );
            }

            static void setProperties() {
                setSpacing();
                setButtonPushing();
                setButtonFrame();
                setFont();
                setTheme();
            }
        
        private:
            static void setSpacing() {
                ImGui::GetStyle().ItemSpacing.x =
                    GuiWindow::properties.getPropertyValue<float>(GuiWindow::GuiWindowProperties::SPASE_X_KEY)
                    * SdlController::SdlProperties::scale.first;

                ImGui::GetStyle().ItemSpacing.y =
                    GuiWindow::properties.getPropertyValue<float>(GuiWindow::GuiWindowProperties::SPASE_Y_KEY)
                    * SdlController::SdlProperties::scale.second;
            }

            static void setButtonPushing() {
                ImGui::PushStyleColor(
                    ImGuiCol_ButtonHovered,
                    GuiWindow::properties.getPropertyValue<ImVec4>(GuiWindow::GuiWindowProperties::BUTTON_HOVERED_COLOR_KEY)
                );
                ImGui::PushStyleColor(
                    ImGuiCol_ButtonActive,
                    GuiWindow::properties.getPropertyValue<ImVec4>(GuiWindow::GuiWindowProperties::BUTTON_ACTIVE_COLOR_KEY)
                );
            }

            static void setButtonFrame() {
                float scaleAvg = std::sqrt(SdlController::SdlProperties::scale.first * SdlController::SdlProperties::scale.second);

                ImGui::GetStyle().FrameRounding =
                    GuiWindow::properties.getPropertyValue<float>(GuiWindow::GuiWindowProperties::FRAME_ROUNDING_KEY)
                    * scaleAvg;

                ImGui::GetStyle().FrameBorderSize =
                    GuiWindow::properties.getPropertyValue<float>(GuiWindow::GuiWindowProperties::FRAME_BORDER_SIZE_KEY)
                    * scaleAvg;

                ImGui::GetStyle().Colors[ImGuiCol_Border] = GuiWindow::properties.getPropertyValue<ImVec4>(GuiWindow::GuiWindowProperties::FRAME_BORDER_COLOR_KEY);
            }

            static void setFont() {
                ImGuiIO& io = ImGui::GetIO();
                std::string fontPath = properties.getPropertyValue<std::string>(GuiWindow::GuiWindowProperties::FONT_KEY);
        
                float scaleAvg = std::sqrt(SdlController::SdlProperties::scale.first * SdlController::SdlProperties::scale.second);
                float fontSize =
                    properties.getPropertyValue<float>(GuiWindow::GuiWindowProperties::FONT_SIZE_KEY)
                    * scaleAvg;
        
                io.Fonts->Clear();
                if (!fontPath.empty()) {
                    io.Fonts->AddFontFromFileTTF(
                        fontPath.c_str(),
                        fontSize,
                        nullptr,
                        io.Fonts->GetGlyphRangesChineseFull()
                    );
                } else {
                    ImFontConfig config;
                    config.SizePixels = fontSize;
        
                    ImFont* font = io.Fonts->AddFontDefault(&config);
                    io.FontDefault = font;
        
                    ImGui_ImplOpenGL3_DestroyFontsTexture();
                    ImGui_ImplOpenGL3_CreateFontsTexture();
                }
            }
        
            static void setTheme() {
                ImguiTheme currentTheme = static_cast<ImguiTheme>(properties.getPropertyValue<int>(GuiWindow::GuiWindowProperties::THEME_KEY));
                switch (currentTheme) {
                    case ImguiTheme::DARK:
                        ImGui::StyleColorsDark();
                        break;
        
                    case ImguiTheme::LIGHT:
                        ImGui::StyleColorsLight();
                        break;
                    
                    case ImguiTheme::CLASSIC:
                        ImGui::StyleColorsClassic();
                        break;
                    
                    default:
                        break;
                }
            }

        protected:
            virtual void draw() = 0;
    };
}