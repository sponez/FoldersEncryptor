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
                std::string outputFilename,
                std::filesystem::path rootPath,
                const std::vector<std::filesystem::path>& filesPaths,
                std::array<char, 256>& password,
                const std::size_t& bufferSize,
                const std::size_t& threadCount,
                std::atomic<std::size_t>* bytesProcessed
            );

            static void processDecryptOption(
                std::filesystem::path outputPath,
                std::filesystem::path decryptedFilePath,
                std::array<char, 256>& password,
                const std::size_t& threadCount,
                std::atomic<std::size_t>* bytesProcessed
            );

            static void processTemporaryDecryptOption(
                std::filesystem::path decryptedFilePath,
                std::array<char, 256>& password,
                const std::size_t& threadCount,
                std::atomic<std::size_t>* bytesProcessed
            );

        private:
            static void secureDelete(const std::filesystem::path& path, const std::size_t& bufferSize);
            static std::filesystem::path createTemporaryDirectory();
    };
}