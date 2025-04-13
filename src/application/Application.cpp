#include "Application.h"
#include "ApplicationConstants.h"
#include "../gui/GuiLayer.h"

#include <SDL_opengl.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <iostream>

// Init block
bool Application::initWindow() {
    window = SDL_CreateWindow(
        APPLICATION_NAME,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        INITIAL_WIDTH,
        INITIAL_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        std::wcerr << L"SDL_GL_CreateWindow Error: " << SDL_GetError() << L'\n';
        return false;
    }

    return true;
}

bool Application::initGlContext() {
    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::wcerr << L"SDL_GL_CreateContext Error: " << SDL_GetError() << L'\n';
        return false;
    }

    return true;
}

void Application::enableVSync() {
    SDL_GL_SetSwapInterval(1);
}

bool Application::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::wcerr << L"SDL_Init Error: " << SDL_GetError() << L'\n';
        return false;
    }

    if (!initWindow()) {
        return false;
    }

    if (!initGlContext()) {
        return false;
    }

    enableVSync();

    return true;
}

bool Application::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/segoeui.ttf", 24.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());

    if (!ImGui_ImplSDL2_InitForOpenGL(window, glContext)) {
        return false;
    }

    if (!ImGui_ImplOpenGL3_Init(GLSL_VERSION)) {
        return false;
    }
    
    return true;
}

bool Application::init() {
    if (!initSDL()) {
        return false;
    }

    if (!initImGui()) {
        return false;
    }

    return true;
}

Application::Application() {
    if (!init()) {
        std::wcerr << L"Initialization error\n";
        running = false;
    }
}

// Destroy block
void Application::destroySDL() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void Application::destroyImGui() {
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

Application::~Application() {
    destroySDL();
    destroyImGui();
}

// Other
void Application::processEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) {
            running = false;
        }
    }
}

void Application::render() {
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    ImGui::Render();
    glViewport(VIEWPORT_X, VIEWPORT_Y, windowWidth, windowHeight);
    glClearColor(CLEAR_COLOR_R, CLEAR_COLOR_G, CLEAR_COLOR_B, CLEAR_COLOR_A);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}

void Application::run() {
    GuiLayer gui;

    while (running) {
        processEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        gui.imGuiWindow();
        this->render();
    }
}