#include "FunctionalGuiWindowController.hpp"

#include "../../../application/Application.hpp"

namespace fe {
    std::optional<GuiWindowId> FunctionalGuiWindowController::process() {
        window->setAndDraw();

        switch (window->action) {
            case FunctionalWindowAction::ENCRYPT: {
                window->action = FunctionalWindowAction::NONE;

                ApplicationRegistry::push(ApplicationRegistry::Key::CURRENT_ACTION, FunctionalWindowAction::ENCRYPT);

                bool useAuthorization =  Application::properties.getPropertyValue<bool>(Application::ApplicationProperties::USE_AUTHORIZATION_FLAG_KEY);
                bool usePerFilePassword = Application::properties.getPropertyValue<bool>(Application::ApplicationProperties::INDIVIDUAL_PASSWORD_FLAG_KEY);
                bool bindToStorageId = Application::properties.getPropertyValue<bool>(Application::ApplicationProperties::BIND_STORAGE_FLAG_KEY);
            
                ApplicationRegistry::push(ApplicationRegistry::Key::AUTHORIZATION_OK, useAuthorization);

                if (bindToStorageId) {
                    ApplicationRegistry::push(
                        ApplicationRegistry::Key::USB_ID,
                        StringUtils::hashString(getStorageDeviceId().value())
                    );
                }

                if (usePerFilePassword) {
                    return GuiWindowId::FILE_PASSWORD;
                }
                
                return GuiWindowId::PROGRESS_BAR;
            }

            case FunctionalWindowAction::DECTYPT: {
                window->action = FunctionalWindowAction::NONE;

                ApplicationRegistry::push(ApplicationRegistry::Key::CURRENT_ACTION, FunctionalWindowAction::DECTYPT);

                auto decryptionInfo = pullEncryptionInfo();
                ApplicationRegistry::push(ApplicationRegistry::Key::AUTHORIZATION_OK, decryptionInfo.authorization);

                if (decryptionInfo.usbFlag) {
                    ApplicationRegistry::push(
                        ApplicationRegistry::Key::USB_ID,
                        StringUtils::hashString(getStorageDeviceId().value())
                    );
                }

                if (decryptionInfo.filePasswordFlag) {
                    return GuiWindowId::FILE_PASSWORD;
                }

                return GuiWindowId::PROGRESS_BAR;
            }

            case FunctionalWindowAction::TEMPORARY_OPEN: {
                window->action = FunctionalWindowAction::NONE;

                ApplicationRegistry::push(ApplicationRegistry::Key::CURRENT_ACTION, FunctionalWindowAction::TEMPORARY_OPEN);

                auto decryptionInfo = pullEncryptionInfo();
                ApplicationRegistry::push(ApplicationRegistry::Key::AUTHORIZATION_OK, decryptionInfo.authorization);

                if (decryptionInfo.usbFlag) {
                    ApplicationRegistry::push(
                        ApplicationRegistry::Key::USB_ID,
                        StringUtils::hashString(getStorageDeviceId().value())
                    );
                }

                if (decryptionInfo.filePasswordFlag) {
                    return GuiWindowId::FILE_PASSWORD;
                }

                return GuiWindowId::PROGRESS_BAR;
            }

            case FunctionalWindowAction::BACK:
                window->action = FunctionalWindowAction::NONE;
                return GuiWindowId::MAIN;

            default:
                return std::nullopt;
        }
    }
}