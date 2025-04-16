#include "Processor.h"

#include <format>
#include <thread>

#include "../controller/encryptor/EncryptionController.h"
#include "../controller/decryptor/DecryptionController.h"
#include "explorer/ExplorerTool.h"

namespace fe {
    void Processor::processEncryptOption(
        std::string outputFilename,
        std::filesystem::path rootPath,
        const std::vector<std::filesystem::path>& filesPaths,
        std::array<char, 256>& password,
        const std::size_t& bufferSize,
        const std::size_t& threadCount,
        std::atomic<std::size_t>* bytesProcessed
    ) {
        EncryptionController::encrypt(
            outputFilename,
            rootPath,
            filesPaths,
            password,
            bufferSize,
            threadCount,
            bytesProcessed
        );

        for (auto path: filesPaths) {
            secureDelete(path, bufferSize);
        }
    }

    void Processor::processDecryptOption(
        std::filesystem::path outputPath,
        std::filesystem::path decryptedFilePath,
        std::array<char, 256>& password,
        const std::size_t& threadCount,
        std::atomic<std::size_t>* bytesProcessed
    ) {
        DecryptionController::decrypt(
            outputPath,
            decryptedFilePath,
            password,
            threadCount,
            bytesProcessed
        );

        std::filesystem::remove_all(decryptedFilePath);
    }

    void Processor::processTemporaryDecryptOption(
        std::filesystem::path decryptedFilePath,
        std::array<char, 256>& password,
        const std::size_t& threadCount,
        std::atomic<std::size_t>* bytesProcessed
    ) {
        std::filesystem::path tempDir = createTemporaryDirectory();
        DecryptionController::decrypt(
            tempDir,
            decryptedFilePath,
            password,
            threadCount,
            bytesProcessed
        );

        ExplorerTool::openPathInExplorer(tempDir);
        while (ExplorerTool::isFolderOpenInExplorer(tempDir)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        std::filesystem::remove_all(tempDir);
    }

    void Processor::secureDelete(const std::filesystem::path& path, const std::size_t& bufferSize) {
        ExplorerTool::removeReadOnlyAttribute(path);

        if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path)) {
            return;
        }

        std::fstream file(path, std::ios::in | std::ios::out | std::ios::binary);
        file.close();
        std::filesystem::remove_all(path);
    }

    std::filesystem::path Processor::createTemporaryDirectory() {
        std::stringstream ss;
        ss << "__fe_temp-" << std::rand() << "__";
        std::filesystem::path tempDir = std::filesystem::temp_directory_path() / ss.str();
        std::filesystem::create_directories(tempDir);
        ExplorerTool::makeFolderHidden(tempDir);

        while (!std::filesystem::exists(tempDir)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        return tempDir;
    }
}
