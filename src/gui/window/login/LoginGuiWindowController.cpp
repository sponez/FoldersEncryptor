#include "LoginGuiWindowController.h"
#include "../../../application/Application.hpp"

namespace fe {
    void LoginGuiWindowController::saveProperties(std::string usernameHash, std::string passwordHash) {
        Application::properties.setPropertyValue<std::string>(Application::ApplicationProperties::USER_KEY, usernameHash);
        Application::properties.setPropertyValue<std::string>(Application::ApplicationProperties::PASSWORD_KEY, passwordHash);
    }
}