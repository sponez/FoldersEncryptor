#pragma once

#include "MainGuiWindow.hpp"
#include "MainWindowAction.hpp"
#include "../GuiWindowId.hpp"
#include "../abstract/GuiWindowController.hpp"

namespace fe {
    class MainGuiWindowController: public GuiWindowController {
        private:
            MainGuiWindowController() = default;
            ~MainGuiWindowController() = default;
            
            MainGuiWindow* window = &MainGuiWindow::getInstance();

        public:
            static MainGuiWindowController& getInstance() {
                static MainGuiWindowController instance;
                return instance;
            }

            std::optional<GuiWindowId> process() override {
                window->setAndDraw();

                switch (window->action)
                {
                    case MainWindowAction::LOGIN:
                        window->action = MainWindowAction::NONE;
                        return GuiWindowId::LOGIN;
                    
                    case MainWindowAction::EXIT:
                        window->action = MainWindowAction::NONE;
                        return GuiWindowId::EXIT;

                    default:
                        return std::nullopt;
                }
            }
    };
}