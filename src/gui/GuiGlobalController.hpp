#pragma once

#include "imgui/ImguiController.h"
#include "sdl/SdlController.h"
#include "window/GuiWindowId.hpp"

#include "window/abstract/GuiWindowController.hpp"
#include "window/main/MainGuiWindowController.hpp"

namespace fe {
    class GuiGlobalController {
        public:
            static void startGui() {
                SdlController::init();
                currentWindowController = &MainGuiWindowController::instance;
                running = true;
                run();
            }

        private:
            inline static bool running = false;
            static GuiWindowController* currentWindowController;

            static void run() {
                while (running) {
                    processSdlEvents();

                    ImGui_ImplOpenGL3_NewFrame();
                    ImGui_ImplSDL2_NewFrame();
                    ImGui::NewFrame();

                    if (auto result = currentWindowController->process()) {
                        switchWindow(result.value());
                    }
                    SdlController::render();
                }
            }

            static void processSdlEvents() {
                SDL_Event event;

                while (SDL_PollEvent(&event)) {
                    ImGui_ImplSDL2_ProcessEvent(&event);

                    switch (event.type)
                    {
                        case SDL_QUIT:
                            processQuitEvent(event);
                            break;

                        case SDL_WINDOWEVENT:
                            processWindowEvent(event);
                            break;

                        default:
                            break;
                    }
                }
            }

            static void processQuitEvent(SDL_Event& event) {
                running = false;
                SdlController::destroy();
            }

            static void processWindowEvent(SDL_Event& event) {
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_MOVED:
                    case SDL_WINDOWEVENT_RESIZED:
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                    case SDL_WINDOWEVENT_DISPLAY_CHANGED:
                        SdlController::updateProperties();
                        break;
                }
            }

            static void switchWindow(GuiWindowId id) {
                switch (id)
                {
                    case GuiWindowId::EXIT:
                        running = false;
                        break;

                    case GuiWindowId::MAIN:
                        currentWindowController = &MainGuiWindowController::instance;
                        break;
                
                    default:
                        break;
                }
            }
    };
}