#pragma once

#include "MainGuiWindow.hpp"
#include "MainWindowAction.hpp"
#include "../GuiWindowId.hpp"
#include "../abstract/GuiWindowController.hpp"

namespace fe {
    class MainGuiWindowController: public GuiWindowController {
        private:
            MainGuiWindowController() {}
            MainGuiWindow* window = &MainGuiWindow::getInstance();

        public:
            static MainGuiWindowController instance;

            std::optional<GuiWindowId> process() override {
                window->draw();

                switch (window->action)
                {
                    case MainWindowAction::PROFILE:
                        break;
                    
                    case MainWindowAction::EXIT:
                        return GuiWindowId::EXIT;

                    default:
                        return std::nullopt;
                }
            }
    };
}