#pragma once

#include <array>
#include <atomic>
#include <filesystem>
#include <thread>
#include <vector>

#include <imgui.h>

namespace fe {
    class GuiWindow {
        public:
            virtual void render();
        
        protected:
            
    };
}