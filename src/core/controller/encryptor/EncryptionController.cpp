#include "EncryptionController.h"

namespace fe {
    void EncryptionController::encrypt(
        std::string outputFilename,
        std::filesystem::path rootPath,
        const std::vector<std::filesystem::path>& filesPaths,
        std::array<char, 256>& password,
        const std::size_t& bufferSize,
        const std::size_t& threadCount,
        std::atomic<std::size_t>* bytesProcessed
    ) {
        auto outputPath = rootPath / outputFilename;
        std::ofstream out(outputPath, std::ios::binary);
        if (!out.is_open()) {
            throw std::runtime_error("Failed to open output file");
        }

        auto salt = generateSalt();
        auto key = ControllerUtils::getKeyAndDestroyPassword(password, salt.get());

        EncryptingWriter writer(out, key, salt, threadCount, bytesProcessed);
        writer.writeSalt(salt.get());
        for (auto filePath: filesPaths) {
            writer.writeFile(rootPath, filePath, bufferSize);
        }
        writer.addEndTag();
        writer.close();

        out.close();
    }

    std::shared_ptr<const unsigned char[]> EncryptionController::generateSalt() {
        auto salt = std::shared_ptr<unsigned char[]>(
            new unsigned char[Chunk::SALT_SIZE],
            std::default_delete<unsigned char[]>()
        );

        randombytes_buf(salt.get(), Chunk::SALT_SIZE);

        return std::static_pointer_cast<const unsigned char[]>(salt);
    }
}