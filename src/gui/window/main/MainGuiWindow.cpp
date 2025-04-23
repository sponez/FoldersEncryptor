#include "MainGuiWindow.hpp"
#include "../../../application/Application.hpp"

namespace fe {
    bool MainGuiWindow::isUsingAuthorization() {
        return Application::properties.getPropertyValue<bool>(Application::ApplicationProperties::USE_AUTHORIZATION_FLAG_KEY);
    }
}