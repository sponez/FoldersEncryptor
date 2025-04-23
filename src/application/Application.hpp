#pragma once

#include <string>

#include "../gui/GuiGlobalController.hpp"
#include "../utils/properties/Properties.hpp"
#include "../utils/properties/PropertiesFileBuilder.hpp"
#include "../utils/properties/PropertiesLoader.hpp"
#include "../utils/storage/StorageChecker.hpp"

namespace fe {
    class Application {
        public:
            class ApplicationProperties: public Properties {
                public:
                    inline static const std::u8string FILE = u8".properties";

                    inline static const std::u8string INDIVIDUAL_PASSWORD_FLAG_KEY = u8"individualPasswordFlag";
                    inline static const bool DEFAULT_INDIVIDUAL_PASSWORD_FLAG_VALUE = false;

                    inline static const std::u8string BIND_STORAGE_FLAG_KEY = u8"bindStorageFlag";
                    inline static const bool DEFAULT_BIND_STORAGE_FLAG_VALUE = false;

                    inline static const std::u8string THREAD_COUNT_KEY = u8"threadCount";
                    inline static const int DEFAULT_THREAD_COUNT_VALUE = 1;

                    inline static const std::u8string BUFFER_SIZE_KEY = u8"bufferSize";
                    inline static const int DEFAULT_BUFFER_SIZE_VALUE = 4096;

                    inline static const std::u8string USER_KEY = u8"user";
                    inline static const std::u8string PASSWORD_KEY = u8"password";
            };
        
            inline static Application::ApplicationProperties properties;

            static void start() {
                loadProperties();
                GuiGlobalController::startGui();
                saveProperties();
            }

            static void saveProperties() {
                std::u8string jsonFile =
                    PropertiesFileBuilder(&properties)
                        .save<bool>(Application::ApplicationProperties::INDIVIDUAL_PASSWORD_FLAG_KEY)
                        ->save<bool>(Application::ApplicationProperties::BIND_STORAGE_FLAG_KEY)
                        ->save<int>(Application::ApplicationProperties::THREAD_COUNT_KEY)
                        ->save<int>(Application::ApplicationProperties::BUFFER_SIZE_KEY)
                        ->build();

                std::filesystem::path savePath = StringUtils::path(Application::ApplicationProperties::FILE);
                std::ofstream out(savePath);
                out << StringUtils::string(jsonFile);
                out.close();
            }

            static void loadProperties() {
                PropertiesLoader loader(Application::ApplicationProperties::FILE, &properties);

                loader.loadOrDefault<bool>(
                    Application::ApplicationProperties::INDIVIDUAL_PASSWORD_FLAG_KEY,
                    Application::ApplicationProperties::DEFAULT_INDIVIDUAL_PASSWORD_FLAG_VALUE
                )->loadOrDefault<bool>(
                    Application::ApplicationProperties::BIND_STORAGE_FLAG_KEY,
                    Application::ApplicationProperties::DEFAULT_BIND_STORAGE_FLAG_VALUE
                )->loadOrDefault<int>(
                    Application::ApplicationProperties::THREAD_COUNT_KEY,
                    Application::ApplicationProperties::DEFAULT_THREAD_COUNT_VALUE
                )->loadOrDefault<int>(
                    Application::ApplicationProperties::BUFFER_SIZE_KEY,
                    Application::ApplicationProperties::DEFAULT_BUFFER_SIZE_VALUE
                );

                if (!isRunningFromRemovableMedia()) {
                    properties.setPropertyValue<bool>(
                        Application::ApplicationProperties::BIND_STORAGE_FLAG_KEY,
                        false
                    );
                }
            }
    };
}