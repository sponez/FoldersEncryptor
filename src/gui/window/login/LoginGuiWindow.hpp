#pragma once

#include <array>
#include <string>

#include <imgui.h>

#include "LoginWindowAction.hpp"
#include "../abstract/GuiWindow.hpp"
#include "../GuiUtils.hpp"

namespace fe {
    class LoginGuiWindow: public GuiWindow {
        private:
            inline static const std::string USERNAME_FIELD = "Username";
            inline static const std::string PASSWORD_FIELD = "Password";
            inline static const std::string OK_BUTTON_NAME = "OK";

            LoginGuiWindow() = default;
            ~LoginGuiWindow() = default;

        public:
            std::array<char, 64> username = std::array<char, 64>();
            std::array<char, 64> password = std::array<char, 64>();

            LoginWindowAction action = LoginWindowAction::NONE;
            static LoginGuiWindow& getInstance() {
                static LoginGuiWindow instance;
                return instance;
            }

        protected:
            void draw() override {
                GuiUtils::renderBackButton(
                    []() { LoginGuiWindow::getInstance().action = LoginWindowAction::BACK; }
                );

                GuiUtils::itemGroup(
                    {
                        std::make_shared<GuiUtils::InputText>(USERNAME_FIELD, username.data(), username.size()),
                        std::make_shared<GuiUtils::InputText>(PASSWORD_FIELD, password.data(), password.size()),
                        std::make_shared<GuiUtils::Button>(OK_BUTTON_NAME, []() {LoginGuiWindow::getInstance().action = LoginWindowAction::HASHING;})
                    }
                );
            }
    };
}