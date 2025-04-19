#pragma once

#include <filesystem>
#include <string>

namespace fe {
    class StringUtils {
        public:
            static std::string string(const std::u8string& u8string) {
                return std::string(u8string.begin(), u8string.end());
            }

            static std::u8string u8string(const std::string& string) {
                return std::u8string(string.begin(), string.end());
            }

            static std::filesystem::path path(const std::u8string& u8string) {
                return std::filesystem::path(u8string);
            }

        private:
            StringUtils() {}
    };
}