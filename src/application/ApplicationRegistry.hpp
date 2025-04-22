#pragma once

#include <atomic>
#include <filesystem>
#include <optional>
#include <vector>

namespace fe {
    class ApplicationRegistry {
        public:
            inline static std::vector<std::filesystem::path> filesToEncrypt;
            inline static std::optional<std::filesystem::path> fileToDecrypt;

            inline static std::atomic<float> progress;

            static void flush() {
                filesToEncrypt.clear();
                fileToDecrypt = std::nullopt;
            }

        private:
            ApplicationRegistry() = default;
    };
}