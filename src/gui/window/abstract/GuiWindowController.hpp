#pragma once

#include <optional>

#include "GuiWindow.hpp"
#include "../GuiWindowId.hpp"

namespace fe {
    class GuiWindowController {
        public:
            virtual std::optional<GuiWindowId> process();
    };
}