#include "Processor.h"

#include <format>
#include <thread>

#include "../controller/encryptor/EncryptionController.h"
#include "../controller/decryptor/DecryptionController.h"
#include "explorer/ExplorerTool.h"

namespace fe {
    void Processor::processEncryptOption(
        const std::string& outputFilename,
        const std::filesystem::path rootPath,
        const std::vector<std::filesystem::path>& filesPaths,
        std::array<char, 256>& password,
        const std::size_t& bufferSize,
        const std::size_t& threadCount
    ) {
        EncryptionController::encrypt(
            outputFilename,
            rootPath,
            filesPaths,
            password,
            bufferSize,
            threadCount
        );

        for (auto path: filesPaths) {
            secureDelete(path, bufferSize);
        }
    }

    void Processor::processDecryptOption(
        const std::filesystem::path outputPath,
        const std::filesystem::path& decryptedFilePath,
        std::array<char, 256>& password
    ) {
        DecryptionController::decrypt(
            outputPath,
            decryptedFilePath,
            password
        );

        std::filesystem::remove_all(decryptedFilePath);
    }

    void Processor::processTemporaryDecryptOption(
        const std::filesystem::path& decryptedFilePath,
        std::array<char, 256>& password
    ) {
        std::filesystem::path tempDir = createTemporaryDirectory();
        DecryptionController::decrypt(
            tempDir,
            decryptedFilePath,
            password
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

        std::uintmax_t size = std::filesystem::file_size(path);
        std::fstream file(path, std::ios::in | std::ios::out | std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to delete file");
        }

        std::vector<char> wipeBuffer(bufferSize, '\0');
        while (size > 0) {
            std::size_t chunk = std::min<std::uintmax_t>(size, bufferSize);
            file.write(wipeBuffer.data(), chunk);
            file.flush();
            size -= chunk;
        }

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
