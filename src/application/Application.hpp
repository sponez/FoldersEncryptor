#pragma once

#include <string>

#include "../gui/GuiGlobalController.hpp"
#include "../utils/properties/Properties.hpp"
#include "../utils/properties/PropertiesFileBuilder.hpp"
#include "../utils/properties/PropertiesLoader.hpp"

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
                std::string jsonFile =
                    PropertiesFileBuilder(&properties)
                        .save<bool>(Application::ApplicationProperties::INDIVIDUAL_PASSWORD_FLAG_KEY)
                        ->save<bool>(Application::ApplicationProperties::BIND_STORAGE_FLAG_KEY)
                        ->build();

                std::filesystem::path savePath = StringUtils::path(Application::ApplicationProperties::FILE);
                std::ofstream out(savePath);
                out << jsonFile;
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
                );
            }
    };
}