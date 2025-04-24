#pragma once

#include "ErrorGuiWindow.hpp"
#include "ErrorWindowAction.hpp"
#include "../GuiWindowId.hpp"
#include "../abstract/GuiWindowController.hpp"

namespace fe {
    class ErrorGuiWindowController: public GuiWindowController {
        private:
            ErrorGuiWindowController() = default;
            ~ErrorGuiWindowController() = default;
            
            ErrorGuiWindow* window = &ErrorGuiWindow::getInstance();

        public:
            static ErrorGuiWindowController& getInstance() {
                static ErrorGuiWindowController instance;
                return instance;
            }

            std::optional<GuiWindowId> process() override {
                window->setAndDraw();

                switch (window->action)
                {
                    case ErrorWindowAction::OK:
                        window->action = ErrorWindowAction::NONE;
                        ApplicationRegistry::flush();
                        return GuiWindowId::FUNCTIONAL;

                    default:
                        return std::nullopt;
                }
            }
    };
}