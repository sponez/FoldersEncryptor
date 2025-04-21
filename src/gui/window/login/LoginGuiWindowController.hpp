#pragma once

#include "LoginGuiWindow.hpp"
#include "LoginWindowAction.hpp"
#include "../GuiWindowId.hpp"
#include "../abstract/GuiWindowController.hpp"

namespace fe {
    class LoginGuiWindowController: public GuiWindowController {
        private:
            LoginGuiWindow* window = &LoginGuiWindow::getInstance();

            LoginGuiWindowController() = default;
            ~LoginGuiWindowController() = default;

        public:
            static LoginGuiWindowController& getInstance() {
                static LoginGuiWindowController instance;
                return instance;
            }

            std::optional<GuiWindowId> process() override {
                window->setAndDraw();

                switch (window->action) {
                    case LoginWindowAction::HASHING:
                        return std::nullopt;

                    default:
                        return std::nullopt;
                }
            }
    };
}