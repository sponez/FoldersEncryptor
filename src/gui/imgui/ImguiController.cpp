#include "ImguiController.h"

#include <math.h>

namespace fe {
    void ImguiController::init(SDL_Window* window, SDL_GLContext* glContext) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
    
        if (!ImGui_ImplSDL2_InitForOpenGL(window, *glContext)) {
            throw std::runtime_error("Failed to init ImGui + SDL2");
        }
    
        if (!ImGui_ImplOpenGL3_Init(StringUtils::string(GLSL_VERSION).c_str())) {
            throw std::runtime_error("Failed to init ImGui + OpenGL");
        }
    }

    void ImguiController::destroy() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }
}