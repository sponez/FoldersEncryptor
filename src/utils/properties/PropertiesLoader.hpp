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
                    properties->setPropertyValue<T>(key, defaultValue);
                    return this;
                }

                std::string keyStr = StringUtils::string(key);
                if (!document.HasMember(keyStr.c_str())) {
                    properties->setPropertyValue<T>(key, defaultValue);
                    return this;
                }

                const auto& jsonValue = document[keyStr.c_str()];
                if constexpr (std::same_as<T, int>) {
                    properties->setPropertyValue<int>(key, jsonValue.GetInt());
                } else if constexpr (std::same_as<T, bool>) {
                    properties->setPropertyValue<bool>(key, jsonValue.GetBool());
                } else if constexpr (std::same_as<T, std::string>) {
                    properties->setPropertyValue<std::string>(key, jsonValue.GetString());
                } else if constexpr (std::same_as<T, float>) {
                    properties->setPropertyValue<float>(key, jsonValue.GetFloat());
                } else if constexpr (std::same_as<T, ImVec4>) {
                    if (!jsonValue.IsArray() || jsonValue.Size() != 4) {
                        properties->setPropertyValue<ImVec4>(key, defaultValue);
                    } else {
                        ImVec4 color(
                            jsonValue[0].GetFloat(),
                            jsonValue[1].GetFloat(),
                            jsonValue[2].GetFloat(),
                            jsonValue[3].GetFloat()
                        );
                        properties->setPropertyValue<ImVec4>(key, color);
                    }
                }

                return this;
            }

        private:
            bool isOpenError = false;
            rapidjson::Document document;
            Properties* properties;
    };
}