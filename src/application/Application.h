#pragma once

#include <string>
#include <variant>

#include <SDL.h>

namespace fe {
    using Property = std::variant<int, bool, std::string>;
}

class Application {
public:
    Application();
    ~Application();

    void run();

private:
    bool initImGui();
    bool init();

    void destroyImGui();

    void processEvents();
    void render();

private:
    SDL_Window* window = nullptr;
    SDL_GLContext glContext = nullptr;
    bool running = true;
};