#pragma once

#include <string>

#include <sodium.h>

#include "FilePasswordGuiWindow.hpp"
#include "FilePasswordWindowAction.hpp"
#include "../GuiWindowId.hpp"
#include "../abstract/GuiWindowController.hpp"
#include "../../../utils/string/StringUtils.hpp"
#include "../../../application/ApplicationRegistry.hpp"

namespace fe {
    class FilePasswordGuiWindowController: public GuiWindowController {
        private:
            FilePasswordGuiWindow* window = &FilePasswordGuiWindow::getInstance();

            FilePasswordGuiWindowController() = default;
            ~FilePasswordGuiWindowController() = default;

        public:
            static FilePasswordGuiWindowController& getInstance() {
                static FilePasswordGuiWindowController instance;
                return instance;
            }

            std::optional<GuiWindowId> process() override {
                window->setAndDraw();

                switch (window->action) {
                    case FilePasswordWindowAction::HASHING: {
                        window->action = FilePasswordWindowAction::NONE;

                        auto filePasswordHash = StringUtils::hashString(std::u8string(window->password.begin(), window->password.end()));
                        sodium_memzero(window->password.data(), window->password.size());
                        ApplicationRegistry::push(ApplicationRegistry::Key::FILE_PASSWORD, filePasswordHash);

                        return GuiWindowId::PROGRESS_BAR;
                    }
                    case FilePasswordWindowAction::BACK:
                        window->action = FilePasswordWindowAction::NONE;
                        ApplicationRegistry::flush();
                        return GuiWindowId::FUNCTIONAL;

                    default:
                        return std::nullopt;
                }
            }
    };
}