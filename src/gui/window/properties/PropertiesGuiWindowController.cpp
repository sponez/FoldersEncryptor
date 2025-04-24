#include "PropertiesGuiWindow.hpp"
#include "PropertiesGuiWindowController.hpp"
#include "../../../application/Application.hpp"

#include "../../../utils/properties/Properties.hpp"

namespace fe {
    void PropertiesGuiWindowController::loadProperties() {
        if (!PropertiesGuiWindow::useAuthorization) {
            PropertiesGuiWindow::useAuthorization =
                Application::properties.getPropertyValue<bool>(Application::ApplicationProperties::USE_AUTHORIZATION_FLAG_KEY);
        }
        if (!PropertiesGuiWindow::usePerFilePassword) {
            PropertiesGuiWindow::usePerFilePassword =
                Application::properties.getPropertyValue<bool>(Application::ApplicationProperties::INDIVIDUAL_PASSWORD_FLAG_KEY);
        }
        if (!PropertiesGuiWindow::bindToStorageId) {
            PropertiesGuiWindow::bindToStorageId =
                Application::properties.getPropertyValue<bool>(Application::ApplicationProperties::BIND_STORAGE_FLAG_KEY);
        }
    }

    void PropertiesGuiWindowController::saveProperties() {
        Application::properties.setPropertyValue<bool>(
            Application::ApplicationProperties::USE_AUTHORIZATION_FLAG_KEY, PropertiesGuiWindow::useAuthorization.value()
        );
        Application::properties.setPropertyValue<bool>(
            Application::ApplicationProperties::INDIVIDUAL_PASSWORD_FLAG_KEY, PropertiesGuiWindow::usePerFilePassword.value()
        );
        Application::properties.setPropertyValue<bool>(
            Application::ApplicationProperties::BIND_STORAGE_FLAG_KEY, PropertiesGuiWindow::bindToStorageId.value()
        );

        Application::properties.removeProperty(Application::ApplicationProperties::USER_KEY);
        Application::properties.removeProperty(Application::ApplicationProperties::PASSWORD_KEY);
    }
}