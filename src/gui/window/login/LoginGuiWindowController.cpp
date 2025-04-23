#include "LoginGuiWindowController.h"
#include "../../../application/Application.hpp"

namespace fe {
    void LoginGuiWindowController::saveProperties(std::u8string usernameHash, std::u8string passwordHash) {
        Application::properties.setPropertyValue<std::u8string>(Application::ApplicationProperties::USER_KEY, usernameHash);
        Application::properties.setPropertyValue<std::u8string>(Application::ApplicationProperties::PASSWORD_KEY, passwordHash);
    }
}