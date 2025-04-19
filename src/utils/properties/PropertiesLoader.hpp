#pragma once

#include <fstream>
#include <sstream>
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include "Properties.hpp"
#include "../string/StringUtils.hpp"

namespace fe {
    class PropertiesLoader {
        public:
            PropertiesLoader(const std::u8string& fileName, Properties* properties): properties(properties) {
                std::ifstream in(StringUtils::path(fileName));
                if (!in) {
                    isOpenError = true;
                    return;
                }

                std::stringstream buffer;
                buffer << in.rdbuf();
                in.close();

                document.Parse(buffer.str().c_str());

                if (document.HasParseError()) {
                    isOpenError = true;
                }
            }
        
            template <SupportedJsonType T>
            PropertiesLoader* loadOrDefault(const std::u8string& key, const T& defaultValue) {
                if (isOpenError) {
                    properties->setPropertyValue(key, defaultValue);
                    return;
                }

                std::string keyStr = StringUtils::string(key);
                if (!document.HasMember(keyStr.c_str())) {
                    properties->setPropertyValue(key, defaultValue);
                    return;
                }

                const auto& jsonValue = document[keyStr.c_str()];
                if constexpr (std::same_as<T, int>) {
                    properties->setPropertyValue(key, jsonValue.GetInt());
                } else if constexpr (std::same_as<T, bool>) {
                    properties->setPropertyValue(key, jsonValue.GetBool());
                } else if constexpr (std::same_as<T, std::string>) {
                    properties->setPropertyValue(key, jsonValue.GetString());
                }

                return this;
            }

        private:
            bool isOpenError = false;
            rapidjson::Document document;
            Properties* properties;
    };
}