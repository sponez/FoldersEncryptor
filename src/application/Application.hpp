#pragma once

#include "../gui/GuiGlobalController.hpp"

namespace fe {
    class Application {
        private:
            Application() {}
        
        public:
            static void start() {
                GuiGlobalController::startGui();
            }
    };
}