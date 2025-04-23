#pragma once

#include <string>

#include <sodium.h>

#include "LoginGuiWindow.hpp"
#include "LoginWindowAction.hpp"
#include "../GuiWindowId.hpp"
#include "../abstract/GuiWindowController.hpp"
#include "../../../utils/string/StringUtils.hpp"

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
                    case LoginWindowAction::HASHING: {
                        window->action = LoginWindowAction::NONE;

                        saveProperties(
                            StringUtils::hashString(std::u8string(window->username.begin(), window->username.end())),
                            StringUtils::hashString(std::u8string(window->password.begin(), window->password.end()))
                        );

                        sodium_memzero(window->username.data(), window->username.size());
                        sodium_memzero(window->password.data(), window->password.size());

                        return GuiWindowId::FUNCTIONAL;
                    }
                    case LoginWindowAction::BACK:
                        window->action = LoginWindowAction::NONE;
                        return GuiWindowId::MAIN;

                    default:
                        return std::nullopt;
                }
            }

        private:
            static void saveProperties(std::u8string usernameHash, std::u8string passwordHash);
    };
}