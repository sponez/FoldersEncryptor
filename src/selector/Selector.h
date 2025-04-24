#pragma once

#include <optional>
#include <string>
#include <vector>
#include <filesystem>

#include <nfd.h>

#include "../utils/string/StringUtils.hpp"

namespace fe {
    class Selector {
        public:
            inline static const std::u8string FILENAME_EXTENSION = u8"fe";
        
            static std::optional<std::vector<std::filesystem::path>> selectFolder();
            static std::optional<std::vector<std::filesystem::path>> selectMultipleFiles();
            static std::optional<std::filesystem::path> selectFeFile();
        
        private:
            Selector() {}
    };
}