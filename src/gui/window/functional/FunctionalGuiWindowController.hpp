#pragma once

#include <sodium.h>

#include "FunctionalGuiWindow.hpp"
#include "FunctionalWindowAction.hpp"
#include "../GuiWindowId.hpp"
#include "../abstract/GuiWindowController.hpp"
#include "../../../core/enctyption/utils/EncryptionInfo.hpp"
#include "../../../utils/storage/StorageChecker.hpp"
#include "../../../utils/string/StringUtils.hpp"

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
        
            std::optional<GuiWindowId> process() override;
    };
}