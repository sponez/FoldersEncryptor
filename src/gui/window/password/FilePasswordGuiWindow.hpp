#pragma once

#include <array>
#include <string>

#include <imgui.h>

#include "FilePasswordWindowAction.hpp"
#include "../abstract/GuiWindow.hpp"
#include "../GuiUtils.hpp"
#include "../../../application/ApplicationRegistry.hpp"

namespace fe {
    class FilePasswordGuiWindow: public GuiWindow {
        private:
            inline static const std::u8string FILE_NAME_FIELD = u8"File name";
            inline static const std::u8string PASSWORD_FIELD = u8"Password";
            inline static const std::u8string OK_BUTTON_NAME = u8"OK";

            FilePasswordGuiWindow() = default;
            ~FilePasswordGuiWindow() = default;

        public:
            std::array<char, 64> fileName = std::array<char, 64>();
            std::array<char, 64> password = std::array<char, 64>();

            FilePasswordWindowAction action = FilePasswordWindowAction::NONE;
            static FilePasswordGuiWindow& getInstance() {
                static FilePasswordGuiWindow instance;
                return instance;
            }

        protected:
            void draw() override {
                GuiUtils::renderBackButton(
                    [&]() { action = FilePasswordWindowAction::BACK; }
                );

                if (ApplicationRegistry::containsAny(ApplicationRegistry::Key::FILES_TO_ENCRYPT)) {
                    GuiUtils::itemGroup(
                        {
                            std::make_shared<GuiUtils::InputText>(FILE_NAME_FIELD, fileName.data(), fileName.size()),
                            std::make_shared<GuiUtils::InputText>(PASSWORD_FIELD, password.data(), password.size()),
                            std::make_shared<GuiUtils::Button>(
                                OK_BUTTON_NAME,
                                [&]() {
                                    action = FilePasswordWindowAction::HASHING;
                                    ApplicationRegistry::push(ApplicationRegistry::Key::OUPUT_FILE_NAME, std::u8string(fileName.begin(), fileName.end()));
                                }
                            )
                        }
                    );
                } else {
                    GuiUtils::itemGroup(
                        {
                            std::make_shared<GuiUtils::InputText>(PASSWORD_FIELD, password.data(), password.size()),
                            std::make_shared<GuiUtils::Button>(OK_BUTTON_NAME, [&]() { action = FilePasswordWindowAction::HASHING; })
                        }
                    );
                }
            }
    };
}