#pragma once

#include <string>

#include <math.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

#include "ImguiTheme.h"
#include "../../sdl/SdlController.h"
#include "../../../utils/properties/Properties.hpp"
#include "../../../utils/properties/PropertiesLoader.hpp"
#include "../../../utils/properties/PropertiesFileBuilder.hpp"
#include "../../../utils/string/StringUtils.hpp"
#include "../../../../resources/fonts/JetBrainsMono_Regular.hpp"

namespace fe {
    class GuiWindow {
        public:
            class GuiWindowProperties: public Properties {
                public:
                    inline static const std::u8string FILE = u8"imgui/.properties";

                    inline static const std::u8string BUTTON_SIZE_KEY = u8"buttonSize";
                    inline static const ImVec2 DEFAULT_BUTTON_SIZE_VALUE = ImVec2(150.0f, 32.0f);

                    inline static const std::u8string SPACE_KEY = u8"space";
                    inline static const ImVec2 DEFAULT_SPACE_VALUE = ImVec2(15.0f, 15.0f);

                    inline static const std::u8string INNER_SPACE_KEY = u8"innerSpace";
                    inline static const ImVec2 DEFAULT_INNER_SPACE_VALUE = ImVec2(7.0f, 7.0f);

                    inline static const std::u8string BUTTON_HOVERED_COLOR_KEY = u8"buttonHoveredColor";
                    inline static const ImVec4 DEFAULT_BUTTON_HOVERED_COLOR_VALUE = ImVec4(0.25f, 0.45f, 0.75f, 1.0f);

                    inline static const std::u8string BUTTON_ACTIVE_COLOR_KEY = u8"buttonActiveColor";
                    inline static const ImVec4 DEFAULT_BUTTON_ACTIVE_COLOR_VALUE = ImVec4(0.15f, 0.35f, 0.55f, 1.0f);

                    inline static const std::u8string FRAME_ROUNDING_KEY = u8"frameRounding";
                    inline static const float DEFAULT_FRAME_ROUNDING_VALUE = 5.0f;

                    inline static const std::u8string FRAME_BORDER_SIZE_KEY = u8"frameBorderSize";
                    inline static const float DEFAULT_FRAME_BORDER_SIZE_VALUE = 1.0f;

                    inline static const std::u8string FRAME_BORDER_COLOR_KEY = u8"frameBorderColor";
                    inline static const ImVec4 DEFAULT_FRAME_BORDER_COLOR_VALUE = ImVec4(1, 1, 1, 0.2f);

                    inline static const std::u8string INPUT_WIDTH_KEY = u8"inputWidth";
                    inline static const float DEFAULT_INPUT_WIDTH_VALUE = 200.0f;

                    inline static const std::u8string FONT_KEY = u8"font";
                    inline static const std::u8string DEFAULT_FONT_VALUE = u8"";

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
                std::u8string jsonFile =
                    PropertiesFileBuilder(&properties)
                        .save<ImVec2>(GuiWindow::GuiWindowProperties::BUTTON_SIZE_KEY)
                        ->save<ImVec2>(GuiWindow::GuiWindowProperties::SPACE_KEY)
                        ->save<ImVec2>(GuiWindow::GuiWindowProperties::INNER_SPACE_KEY)
                        ->save<ImVec4>(GuiWindow::GuiWindowProperties::BUTTON_HOVERED_COLOR_KEY)
                        ->save<ImVec4>(GuiWindow::GuiWindowProperties::BUTTON_ACTIVE_COLOR_KEY)
                        ->save<float>(GuiWindow::GuiWindowProperties::FRAME_ROUNDING_KEY)
                        ->save<float>(GuiWindow::GuiWindowProperties::FRAME_BORDER_SIZE_KEY)
                        ->save<ImVec4>(GuiWindow::GuiWindowProperties::FRAME_BORDER_COLOR_KEY)
                        ->save<float>(GuiWindow::GuiWindowProperties::INPUT_WIDTH_KEY)
                        ->save<std::u8string>(GuiWindow::GuiWindowProperties::FONT_KEY)
                        ->save<float>(GuiWindow::GuiWindowProperties::FONT_SIZE_KEY)
                        ->save<int>(GuiWindow::GuiWindowProperties::THEME_KEY)
                        ->build();
    
                std::filesystem::path savePath = StringUtils::path(GuiWindow::GuiWindowProperties::FILE);
                std::filesystem::create_directory(savePath.parent_path());
                std::ofstream out(savePath);
                out << StringUtils::string(jsonFile);
                out.close();
            }

            static void loadProperties() {
                PropertiesLoader loader(GuiWindow::GuiWindowProperties::FILE, &properties);

                loader.loadOrDefault<ImVec2>(
                    GuiWindow::GuiWindowProperties::BUTTON_SIZE_KEY,
                    GuiWindow::GuiWindowProperties::DEFAULT_BUTTON_SIZE_VALUE
                )->loadOrDefault<ImVec2>(
                    GuiWindow::GuiWindowProperties::SPACE_KEY,
                    GuiWindow::GuiWindowProperties::DEFAULT_SPACE_VALUE
                )->loadOrDefault<ImVec2>(
                    GuiWindow::GuiWindowProperties::INNER_SPACE_KEY,
                    GuiWindow::GuiWindowProperties::DEFAULT_INNER_SPACE_VALUE
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
                )->loadOrDefault<std::u8string>(
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
                ImGuiStyle& style = ImGui::GetStyle();
                ImVec2 space = GuiWindow::properties.getPropertyValue<ImVec2>(GuiWindow::GuiWindowProperties::SPACE_KEY);
                ImVec2 innerSpace = GuiWindow::properties.getPropertyValue<ImVec2>(GuiWindow::GuiWindowProperties::INNER_SPACE_KEY);

                style.ItemSpacing = ImVec2(space.x * SdlController::SdlProperties::scale.first, space.y * SdlController::SdlProperties::scale.second);
                style.ItemInnerSpacing = ImVec2(innerSpace.x * SdlController::SdlProperties::scale.first, innerSpace.y * SdlController::SdlProperties::scale.second);
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
                std::u8string fontPath = properties.getPropertyValue<std::u8string>(GuiWindow::GuiWindowProperties::FONT_KEY);
        
                float scaleAvg = std::sqrt(SdlController::SdlProperties::scale.first * SdlController::SdlProperties::scale.second);
                float fontSize =
                    properties.getPropertyValue<float>(GuiWindow::GuiWindowProperties::FONT_SIZE_KEY)
                    * scaleAvg;
        
                io.Fonts->Clear();

                ImVector<ImWchar> ranges;
                ImFontGlyphRangesBuilder builder;
                builder.AddText("↩");
                builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
                builder.BuildRanges(&ranges);

                if (!fontPath.empty()) {
                    io.Fonts->AddFontFromFileTTF(
                        StringUtils::string(fontPath).c_str(),
                        fontSize,
                        nullptr,
                        ranges.Data
                    );
                } else {
                    ImFontConfig fontCfg;
                    fontCfg.FontDataOwnedByAtlas = false;

                    io.Fonts->AddFontFromMemoryTTF(
                        JetBrainsMono_Regular_ttf,
                        JetBrainsMono_Regular_ttf_len,
                        fontSize,
                        &fontCfg,
                        ranges.Data
                    );
                }

                ImGui_ImplOpenGL3_DestroyFontsTexture();
                ImGui_ImplOpenGL3_CreateFontsTexture();
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