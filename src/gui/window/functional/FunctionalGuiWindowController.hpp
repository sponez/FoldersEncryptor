#pragma once

#include "FunctionalGuiWindow.hpp"
#include "FunctionalWindowAction.hpp"
#include "../GuiWindowId.hpp"
#include "../abstract/GuiWindowController.hpp"

namespace fe {
    class FunctionalGuiWindowController: public GuiWindowController {
        private:
            FunctionalGuiWindow* window = &FunctionalGuiWindow::getInstance();

            FunctionalGuiWindowController() = default;
            ~FunctionalGuiWindowController() = default;
        
        public:
            static FunctionalGuiWindowController& getInstance() {
                static FunctionalGuiWindowController instance;
                return instance;
            }
        
            std::optional<GuiWindowId> process() override {
                window->setAndDraw();

                switch (window->action) {
                    case FunctionalWindowAction::ENCRYPT:
                        window->action = FunctionalWindowAction::NONE;
                        return std::nullopt;

                    case FunctionalWindowAction::DECTYPT:
                        window->action = FunctionalWindowAction::NONE;
                        return std::nullopt;

                    case FunctionalWindowAction::TEMPORARY_OPEN:
                        window->action = FunctionalWindowAction::NONE;
                        return std::nullopt;

                    case FunctionalWindowAction::PROPERTIES:
                        window->action = FunctionalWindowAction::NONE;
                        return std::nullopt;

                    case FunctionalWindowAction::BACK:
                        window->action = FunctionalWindowAction::NONE;
                        return GuiWindowId::MAIN;

                    default:
                        return std::nullopt;
                }
            }
    };
}