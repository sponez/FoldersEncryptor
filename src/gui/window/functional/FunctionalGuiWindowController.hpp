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

                bool bindToStorageId = Application::properties.getPropertyValue<bool>(Application::ApplicationProperties::BIND_STORAGE_FLAG_KEY);
                switch (window->action) {
                    case FunctionalWindowAction::ENCRYPT: {
                        window->action = FunctionalWindowAction::NONE;

                        bool usePerFilePassword = Application::properties.getPropertyValue<bool>(Application::ApplicationProperties::INDIVIDUAL_PASSWORD_FLAG_KEY);
                        if (usePerFilePassword) {
                            return GuiWindowId::FILE_PASSWORD;
                        }
                        
                        return GuiWindowId::PROGRESS_BAR;
                    }

                    case FunctionalWindowAction::DECTYPT:
                        window->action = FunctionalWindowAction::NONE;
                        return GuiWindowId::PROGRESS_BAR;

                    case FunctionalWindowAction::TEMPORARY_OPEN:
                        window->action = FunctionalWindowAction::NONE;
                        return GuiWindowId::PROGRESS_BAR;

                    case FunctionalWindowAction::PROPERTIES:
                        window->action = FunctionalWindowAction::NONE;
                        return GuiWindowId::PROPERTIES;

                    case FunctionalWindowAction::BACK:
                        window->action = FunctionalWindowAction::NONE;
                        return GuiWindowId::MAIN;

                    default:
                        return std::nullopt;
                }
            }
    };
}