#pragma once

#include <filesystem>
#include <vector>

#include <windows.h>
#include <shellapi.h>
#include <atomic>

namespace fe {
    class Processor {
        public:
            static void processEncryptOption(
                std::u8string outputFilename,
                std::filesystem::path rootPath,
                const std::vector<std::filesystem::path>& filesPaths
            );

            static void processDecryptOption(
                std::filesystem::path outputPath,
                std::filesystem::path decryptedFilePath
            );

            static void processTemporaryDecryptOption(std::filesystem::path decryptedFilePath);

        private:
            static void secureDelete(const std::filesystem::path& path);
            static std::filesystem::path createTemporaryDirectory(std::filesystem::path path);
    };
}