#pragma once

#include <filesystem>
#include <sstream>
#include <string>

#include <sodium.h>

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

            static std::u8string hashString(std::u8string input) {
                unsigned char hash[crypto_generichash_BYTES];

                std::string inputStr = string(input);
                crypto_generichash(
                    hash, sizeof(hash),
                    reinterpret_cast<const unsigned char*>(inputStr.data()), inputStr.size(),
                    nullptr, 0
                );
            
                std::stringstream ss;
                for (size_t i = 0; i < sizeof(hash); ++i) {
                    ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
                }

                return u8string(ss.str());
            }

        private:
            StringUtils() {}
    };
}