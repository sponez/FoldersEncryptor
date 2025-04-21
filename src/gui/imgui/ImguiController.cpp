#include "ImguiController.h"

namespace fe {
    void ImguiController::init(SDL_Window* window, SDL_GLContext* glContext) {
        loadProperties();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
    
        if (!ImGui_ImplSDL2_InitForOpenGL(window, *glContext)) {
            throw std::runtime_error("Failed to init ImGui + SDL2");
        }
    
        if (!ImGui_ImplOpenGL3_Init(StringUtils::string(ImguiController::ImguiProperties::GLSL_VERSION).c_str())) {
            throw std::runtime_error("Failed to init ImGui + OpenGL");
        }

        setFont();
        setTheme();
    }

    void ImguiController::destroy() {
        saveProperties();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    void ImguiController::saveProperties() {
        std::string jsonFile =
            PropertiesFileBuilder(&properties)
                .save<std::string>(ImguiController::ImguiProperties::FONT_KEY)
                ->save<float>(ImguiController::ImguiProperties::FONT_SIZE_KEY)
                ->save<int>(ImguiController::ImguiProperties::THEME_KEY)
                ->build();

        std::filesystem::path savePath = StringUtils::path(ImguiController::ImguiProperties::FILE);
        std::filesystem::create_directory(savePath.parent_path());
        std::ofstream out(savePath);
        out << jsonFile;
        out.close();
    }
    
    void ImguiController::loadProperties() {
        PropertiesLoader loader(ImguiController::ImguiProperties::FILE, &properties);

        loader.loadOrDefault<std::string>(
            ImguiController::ImguiProperties::FONT_KEY,
            ImguiController::ImguiProperties::DEFAULT_FONT_VALUE
        )->loadOrDefault<float>(
            ImguiController::ImguiProperties::FONT_SIZE_KEY,
            ImguiController::ImguiProperties::DEFAULT_FONT_SIZE_VALUE
        )->loadOrDefault<int>(
            ImguiController::ImguiProperties::THEME_KEY,
            static_cast<int>(ImguiController::ImguiProperties::DEFAULT_THEME_VALUE)
        );
    }

    void ImguiController::setFont() {
        ImGuiIO& io = ImGui::GetIO();
        std::string fontPath = properties.getPropertyValue<std::string>(ImguiController::ImguiProperties::FONT_KEY);
        float fontSize = properties.getPropertyValue<float>(ImguiController::ImguiProperties::FONT_SIZE_KEY) * SdlController::SdlProperties::scale.second;

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

    void ImguiController::setTheme() {
        ImguiTheme currentTheme = static_cast<ImguiTheme>(properties.getPropertyValue<int>(ImguiController::ImguiProperties::THEME_KEY));
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
}