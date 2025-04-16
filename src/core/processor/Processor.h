#pragma once

#include <filesystem>
#include <vector>

#include <windows.h>
#include <shellapi.h>

namespace fe {
    class Processor {
        public:
            static void processEncryptOption(
                const std::string& outputFilename,
                const std::filesystem::path rootPath,
                const std::vector<std::filesystem::path>& filesPaths,
                std::array<char, 256>& password,
                const std::size_t& bufferSize,
                const std::size_t& threadCount
            );

            static void processDecryptOption(
                const std::filesystem::path outputPath,
                const std::filesystem::path& decryptedFilePath,
                std::array<char, 256>& password,
                const std::size_t& threadCount
            );

            static void processTemporaryDecryptOption(
                const std::filesystem::path& decryptedFilePath,
                std::array<char, 256>& password,
                const std::size_t& threadCount
            );

        private:
            static void secureDelete(const std::filesystem::path& path, const std::size_t& bufferSize);
            static std::filesystem::path createTemporaryDirectory();
            static DWORD launchExplorerForFolder(const std::filesystem::path& folder);
            static bool waitForProcessExit(DWORD pid, int timeoutSeconds = 600);
    };
}