#pragma once

#include <map>
#include <string>

#include "../../application/Application.h"

namespace fe {
    template <typename T>
    concept SupportedJsonType =
        std::same_as<T, int> ||
        std::same_as<T, float> ||
        std::same_as<T, bool> ||
        std::same_as<T, std::string>;

    class Properties {
        private:
            std::map<std::u8string, Property> state;

        public:
            template<SupportedJsonType T>
            T& getPropertyValue(const std::u8string& propertyName) {
                auto it = state.find(propertyName);

                if (it == state.end()) {
                    throw std::runtime_error("Property not found: " + propertyName);
                }

                if (auto* ptr = std::get_if<T>(&(it->second))) {
                    return *ptr;
                }

                throw std::runtime_error("Wrong type for property: " + propertyName);
            }

            template<SupportedJsonType T>
            void setPropertyValue(const std::u8string& propertyName, const T& value) {
                state[propertyName] = value;
            }
    };
}