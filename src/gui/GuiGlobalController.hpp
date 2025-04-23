#pragma once

#include <math.h>

#include "imgui/ImguiController.h"
#include "sdl/SdlController.h"
#include "window/GuiWindowId.hpp"

#include "window/abstract/GuiWindow.hpp"
#include "window/abstract/GuiWindowController.hpp"
#include "window/main/MainGuiWindowController.hpp"
#include "window/login/LoginGuiWindowController.h"
#include "window/functional/FunctionalGuiWindowController.hpp"
#include "window/properties/PropertiesGuiWindowController.hpp"
#include "window/password/FilePasswordGuiWindowController.hpp"
#include "window/progressbar/ProgressBarGuiWindowController.hpp"

namespace fe {
    class GuiGlobalController {
        public:
            static void startGui() {
                SdlController::init();
                GuiWindow::loadProperties();
                GuiWindow::setProperties();

                currentWindowController = &MainGuiWindowController::getInstance();
                running = true;
                run();

                GuiWindow::saveProperties();
                SdlController::destroy();
            }

        private:
            inline static bool running = false;
            inline static GuiWindowController* currentWindowController = nullptr;

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
            }

            static void processWindowEvent(SDL_Event& event) {
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_MOVED:
                    case SDL_WINDOWEVENT_RESIZED:
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                    case SDL_WINDOWEVENT_DISPLAY_CHANGED:
                        SdlController::updateProperties();
                        SdlController::findScale();
                        GuiWindow::setProperties();
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
                        currentWindowController = &MainGuiWindowController::getInstance();
                        break;
                
                    case GuiWindowId::LOGIN:
                        currentWindowController = &LoginGuiWindowController::getInstance();
                        break;
                    
                    case GuiWindowId::FUNCTIONAL:
                        currentWindowController = &FunctionalGuiWindowController::getInstance();
                        break;
                    
                    case GuiWindowId::PROPERTIES:
                        currentWindowController = &PropertiesGuiWindowController::getInstance();
                        break;
                    
                    case GuiWindowId::FILE_PASSWORD:
                        currentWindowController = &FilePasswordGuiWindowController::getInstance();
                        break;

                    case GuiWindowId::PROGRESS_BAR:
                        currentWindowController = &ProgressBarGuiWindowController::getInstance();
                        break;

                    default:
                        break;
                }
            }
    };
}