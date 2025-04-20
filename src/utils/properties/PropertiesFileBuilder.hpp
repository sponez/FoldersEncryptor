#pragma once

#include <concepts>
#include <string>
#include <type_traits>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include "Properties.hpp"
#include "../string/StringUtils.hpp"

namespace fe {
    class PropertiesFileBuilder {
        public:
            PropertiesFileBuilder(Properties* properties): properties(properties) {
                document.SetObject();
                allocator = document.GetAllocator();
            }

            template <SupportedJsonType T>
            PropertiesFileBuilder* save(const std::u8string& key) {
                std::string keyStr = StringUtils::string(key);
                rapidjson::Value nameValue(keyStr.c_str(), allocator);

                if constexpr (std::is_same_v<T, std::string>) {
                    std::string value = properties->getPropertyValue<std::string>(key);
                    rapidjson::Value valueValue(value.c_str(), allocator);
                    document.AddMember(nameValue, valueValue, allocator);
                } else {
                    T value = properties->getPropertyValue<T>(key);
                    rapidjson::Value valueValue(value);
                    document.AddMember(nameValue, valueValue, allocator);
                }

                return this;
            }

            std::string build() {
                rapidjson::StringBuffer buffer;
                rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
                document.Accept(writer);

                return buffer.GetString();
            }

        private:
            Properties* properties;
            rapidjson::Document document;
            rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> allocator;
    };
}