#include "Processor.h"

#include <format>
#include <thread>

#include "../controller/encryptor/EncryptionController.h"
#include "../controller/decryptor/DecryptionController.h"
#include "explorer/ExplorerTool.h"

namespace fe {
    void Processor::processEncryptOption(
        std::u8string outputFilename,
        std::filesystem::path rootPath,
        const std::vector<std::filesystem::path>& filesPaths
    ) {
        EncryptionController::encrypt(
            outputFilename,
            rootPath,
            filesPaths
        );

        for (auto path: filesPaths) {
            secureDelete(path);
        }
    }

    void Processor::processDecryptOption(
        std::filesystem::path outputPath,
        std::filesystem::path decryptedFilePath
    ) {
        DecryptionController::decrypt(
            outputPath,
            decryptedFilePath
        );

        std::filesystem::remove_all(decryptedFilePath);
    }

    void Processor::processTemporaryDecryptOption(std::filesystem::path decryptedFilePath) {
        std::filesystem::path tempDir = createTemporaryDirectory();
        DecryptionController::decrypt(
            tempDir,
            decryptedFilePath
        );

        ExplorerTool::openPathInExplorer(tempDir);
        while (ExplorerTool::isFolderOpenInExplorer(tempDir)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        std::filesystem::remove_all(tempDir);
    }

    void Processor::secureDelete(const std::filesystem::path& path) {
        ExplorerTool::removeReadOnlyAttribute(path);

        if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path)) {
            return;
        }

        std::ofstream file(path);
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
