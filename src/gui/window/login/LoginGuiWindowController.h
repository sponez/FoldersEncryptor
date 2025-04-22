#pragma once

#include <string>

#include <sodium.h>

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

            std::string hashString(std::array<char, 64> input) {
                unsigned char hash[crypto_generichash_BYTES];
                crypto_generichash(
                    hash, sizeof(hash),
                    reinterpret_cast<const unsigned char*>(input.data()), input.size(),
                    nullptr, 0
                );
            
                std::stringstream ss;
                for (size_t i = 0; i < sizeof(hash); ++i) {
                    ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
                }

                return ss.str();
            }

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

                        saveProperties(hashString(window->username), hashString(window->password));

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
            static void saveProperties(std::string usernameHash, std::string passwordHash);
    };
}