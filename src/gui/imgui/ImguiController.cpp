#include "ImguiController.h"

namespace fe {
    void ImguiController::init(SDL_Window* window, SDL_GLContext* glContext) {
        loadProperties();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        setFont();
        setTheme();
    
        if (!ImGui_ImplSDL2_InitForOpenGL(window, *glContext)) {
            throw std::runtime_error("Failed to init ImGui + SDL2");
        }
    
        if (!ImGui_ImplOpenGL3_Init(StringUtils::string(ImguiController::ImguiProperties::GLSL_VERSION).c_str())) {
            throw std::runtime_error("Failed to init ImGui + OpenGL");
        }
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
        io.Fonts->AddFontFromFileTTF(
            properties.getPropertyValue<std::string>(ImguiController::ImguiProperties::FONT_KEY).c_str(),
            properties.getPropertyValue<float>(ImguiController::ImguiProperties::FONT_SIZE_KEY),
            nullptr,
            io.Fonts->GetGlyphRangesChineseFull()
        );
    }

    void ImguiController::setTheme() {
        ImguiTheme currentTheme =   static_cast<ImguiTheme>(properties.getPropertyValue<int>(ImguiController::ImguiProperties::THEME_KEY));
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