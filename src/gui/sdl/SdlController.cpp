#include "SdlController.h"
#include "../imgui/ImguiController.h"

namespace fe {
    void SdlController::init() {
        loadProperties();
        createWindow();
        createGlContext();
        enableVSync();
        findScale();
        ImguiController::init(window, &glContext);
    }

    void SdlController::destroy() {
        saveProperties();
        
        ImguiController::destroy();
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void SdlController::saveProperties() {
        std::string jsonFile =
            PropertiesFileBuilder(&properties)
                .save<int>(SdlController::SdlProperties::MONITOR_ID_KEY)
                ->save<int>(SdlController::SdlProperties::WINDOW_POSITION_X_KEY)
                ->save<int>(SdlController::SdlProperties::WINDOW_POSITION_Y_KEY)
                ->save<int>(SdlController::SdlProperties::WINDOW_WIDTH_KEY)
                ->save<int>(SdlController::SdlProperties::WINDOW_HEIGHT_KEY)
                ->build();

        std::filesystem::path savePath = StringUtils::path(SdlController::SdlProperties::FILE);
        std::filesystem::create_directory(savePath.parent_path());
        std::ofstream out(savePath);
        out << jsonFile;
        out.close();
    }

    void SdlController::loadProperties() {
        PropertiesLoader loader(SdlController::SdlProperties::FILE, &properties);

        loader.loadOrDefault<int>(
            SdlController::SdlProperties::MONITOR_ID_KEY,
            SdlController::SdlProperties::DEFAULT_MONITOR_ID_VALUE
        )->loadOrDefault<int>(
            SdlController::SdlProperties::WINDOW_POSITION_X_KEY,
            SdlController::SdlProperties::DEFAULT_WINDOW_POSITION_X_VALUE
        )->loadOrDefault<int>(
            SdlController::SdlProperties::WINDOW_POSITION_Y_KEY,
            SdlController::SdlProperties::DEFAULT_WINDOW_POSITION_Y_VALUE
        )->loadOrDefault<int>(
            SdlController::SdlProperties::WINDOW_WIDTH_KEY,
            SdlController::SdlProperties::DEFAULT_WINDOW_WIDTH_VALUE
        )->loadOrDefault<int>(
            SdlController::SdlProperties::WINDOW_HEIGHT_KEY,
            SdlController::SdlProperties::DEFAULT_WINDOW_HEIGHT_VALUE
        );
    }

    void SdlController::updateProperties() {
        int displayIndex;
        SDL_Rect bounds;
        std::pair<int, int> windowSize;
        std::pair<int, int> windowGlobalPosition;
        std::pair<int, int> windowRelativePosition;

        displayIndex = SDL_GetWindowDisplayIndex(window);
        SDL_GetDisplayBounds(displayIndex, &bounds);
        SDL_GetWindowSize(window, &windowSize.first, &windowSize.second);
        SDL_GetWindowPosition(window, &windowGlobalPosition.first, &windowGlobalPosition.second);
        windowRelativePosition.first = windowGlobalPosition.first - bounds.x;
        windowRelativePosition.second = windowGlobalPosition.second - bounds.y;

        properties.setPropertyValue<int>(SdlController::SdlProperties::MONITOR_ID_KEY, displayIndex);
        properties.setPropertyValue<int>(SdlController::SdlProperties::WINDOW_POSITION_X_KEY, windowRelativePosition.first);
        properties.setPropertyValue<int>(SdlController::SdlProperties::WINDOW_POSITION_Y_KEY, windowRelativePosition.second);
        properties.setPropertyValue<int>(SdlController::SdlProperties::WINDOW_WIDTH_KEY, windowSize.first);
        properties.setPropertyValue<int>(SdlController::SdlProperties::WINDOW_HEIGHT_KEY, windowSize.second);
    }

    void SdlController::createWindow() {
        SDL_Rect bounds;

        SDL_GetDisplayBounds(
            properties.getPropertyValue<int>(SdlController::SdlProperties::MONITOR_ID_KEY),
            &bounds
        );

        window = SDL_CreateWindow(
            StringUtils::string(SdlController::SdlProperties::APPLICATION_NAME).c_str(),
            properties.getPropertyValue<int>(SdlController::SdlProperties::WINDOW_POSITION_X_KEY) + bounds.x,
            properties.getPropertyValue<int>(SdlController::SdlProperties::WINDOW_POSITION_Y_KEY) + bounds.y,
            properties.getPropertyValue<int>(SdlController::SdlProperties::WINDOW_WIDTH_KEY),
            properties.getPropertyValue<int>(SdlController::SdlProperties::WINDOW_HEIGHT_KEY),
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
        );

        if (!window) {
            throw std::runtime_error("Faled to create window");
        }
    }

    void SdlController::createGlContext() {
        glContext = SDL_GL_CreateContext(window);

        if (!glContext) {
            throw std::runtime_error("Faled to create GL Context");
        }
    }

    void SdlController::enableVSync() {
        SDL_GL_SetSwapInterval(1);
    }

    void SdlController::findScale()
    {
        int windowW, windowH, drawableW, drawableH;
        SDL_GetWindowSize(window, &windowW, &windowH);
        SDL_GL_GetDrawableSize(window, &drawableW, &drawableH);

        SdlController::SdlProperties::scale = float(drawableW) / float(windowW);
    }
}