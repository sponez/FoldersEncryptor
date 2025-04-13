#pragma once

#include <SDL.h>

class Application {
public:
    Application();
    ~Application();

    void run();

private:
    bool initWindow();
    bool initGlContext();
    void enableVSync();
    bool initSDL();
    bool initImGui();
    bool init();

    void destroySDL();
    void destroyImGui();

    void processEvents();
    void render();

private:
    SDL_Window* window = nullptr;
    SDL_GLContext glContext = nullptr;
    bool running = true;
};