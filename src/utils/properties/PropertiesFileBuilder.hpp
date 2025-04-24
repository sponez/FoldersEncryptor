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
                rapidjson::Value nameValue(StringUtils::string(key).c_str(), allocator);

                if constexpr (std::is_same_v<T, std::u8string>) {
                    std::u8string value = properties->getPropertyValue<std::u8string>(key);
                    rapidjson::Value valueValue(StringUtils::string(value).c_str(), allocator);
                    document.AddMember(nameValue, valueValue, allocator);
                } else if constexpr (std::is_same_v<T, ImVec2>) {
                    ImVec2 value = properties->getPropertyValue<ImVec2>(key);
                    rapidjson::Value valueValue(rapidjson::kArrayType);
                    valueValue.PushBack(value.x, allocator);
                    valueValue.PushBack(value.y, allocator);
                    document.AddMember(nameValue, valueValue, allocator);
                } else if constexpr (std::is_same_v<T, ImVec4>) {
                    ImVec4 value = properties->getPropertyValue<ImVec4>(key);
                    rapidjson::Value valueValue(rapidjson::kArrayType);
                    valueValue.PushBack(value.x, allocator);
                    valueValue.PushBack(value.y, allocator);
                    valueValue.PushBack(value.z, allocator);
                    valueValue.PushBack(value.w, allocator);
                    document.AddMember(nameValue, valueValue, allocator);
                } else {
                    T value = properties->getPropertyValue<T>(key);
                    rapidjson::Value valueValue(value);
                    document.AddMember(nameValue, valueValue, allocator);
                }

                return this;
            }

            std::u8string build() {
                rapidjson::StringBuffer buffer;
                rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
                document.Accept(writer);

                return StringUtils::u8string(buffer.GetString());
            }

        private:
            Properties* properties;
            rapidjson::Document document;
            rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> allocator;
    };
}