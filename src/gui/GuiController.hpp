#pragma once

namespace fe {
    class GuiController {
        private:
            inline static const char* GLSL_VERSION = "#version 130";
            
            inline static const int INITIAL_WIDTH = 800;
            inline static const int INITIAL_HEIGHT = 600;
            
            inline static const int VIEWPORT_X = 0;
            inline static const int VIEWPORT_Y = 0;
            
            inline static const float CLEAR_COLOR_R = 0.1f;
            inline static const float CLEAR_COLOR_G = 0.1f;
            inline static const float CLEAR_COLOR_B = 0.1f;
            inline static const float CLEAR_COLOR_A = 1.0f;

        public:
            static void initGui();
    };
}