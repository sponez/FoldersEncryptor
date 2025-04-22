#pragma once

#include <map>
#include <string>
#include <variant>

#include <imgui.h>

#include "../string/StringUtils.hpp"

namespace fe {
    using Property = std::variant<int, float, bool, std::string, ImVec2, ImVec4>;

    template <typename T>
    concept SupportedJsonType =
        std::same_as<T, int> ||
        std::same_as<T, float> ||
        std::same_as<T, bool> ||
        std::same_as<T, std::string> ||
        std::same_as<T, ImVec2> ||
        std::same_as<T, ImVec4>;

    class Properties {
        private:
            std::map<std::u8string, Property> state;

        public:
            template<SupportedJsonType T>
            T& getPropertyValue(const std::u8string& propertyName) {
                auto it = state.find(propertyName);

                if (it == state.end()) {
                    throw std::runtime_error("Property not found: " + StringUtils::string(propertyName));
                }

                if (auto* ptr = std::get_if<T>(&(it->second))) {
                    return *ptr;
                }

                throw std::runtime_error("Wrong type for property: " + StringUtils::string(propertyName));
            }

            template<SupportedJsonType T>
            void setPropertyValue(const std::u8string& propertyName, const T& value) {
                state[propertyName] = value;
            }
    };
}