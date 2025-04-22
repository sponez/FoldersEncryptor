#pragma once

#include <string>

#include <sodium.h>

#include "PropertiesGuiWindow.hpp"
#include "PropertiesWindowAction.hpp"
#include "../GuiWindowId.hpp"
#include "../abstract/GuiWindowController.hpp"

namespace fe {
    class PropertiesGuiWindowController: public GuiWindowController {
        private:
            PropertiesGuiWindow* window = &PropertiesGuiWindow::getInstance();

            PropertiesGuiWindowController() = default;
            ~PropertiesGuiWindowController() = default;
        
        public:
            static PropertiesGuiWindowController& getInstance() {
                static PropertiesGuiWindowController instance;
                return instance;
            }
        
            std::optional<GuiWindowId> process() override {
                loadProperties();
                window->setAndDraw();

                switch (window->action) {
                    case PropertiesWindowAction::APPLY:
                        window->action = PropertiesWindowAction::NONE;
                        saveProperties();
                        return std::nullopt;
                    
                    case PropertiesWindowAction::BACK:
                    case PropertiesWindowAction::CANCEL:
                        window->action = PropertiesWindowAction::NONE;
                        resetProperties();
                        return GuiWindowId::FUNCTIONAL;

                    case PropertiesWindowAction::RESTORE:
                        window->action = PropertiesWindowAction::NONE;
                        return std::nullopt;

                    default:
                        return std::nullopt;
                }
            }
        
        private:
            static void loadProperties();
            static void saveProperties();
            static void resetProperties() {
                PropertiesGuiWindow::usePerFilePassword = std::nullopt;
                PropertiesGuiWindow::bindToStorageId = std::nullopt;
            }
    };
}