#include "EncryptionController.h"

namespace fe {
    void EncryptionController::encrypt(
        const std::string& outputFilename,
        const std::filesystem::path rootPath,
        const std::vector<std::filesystem::path>& filesPaths,
        std::array<char, 256>& password,
        const std::size_t& bufferSize
    ) {
        auto outputPath = FilesUtils::determineOutputPath(filesPaths, outputFilename);
        std::ofstream out(outputPath, std::ios::binary);
        if (!out.is_open()) {
            throw std::runtime_error("Failed to open output file");
        }

        auto salt = generateSalt();
        auto key = ControllerUtils::getKeyAndDestroyPassword(password, salt.get());
        auto header = std::make_unique<const unsigned char[]>(Chunk::HEADER_LENGTH);
        auto context = std::make_unique<SecretStreamContext>();
        if (crypto_secretstream_xchacha20poly1305_init_push(context.get(), const_cast<unsigned char*>(header.get()), key.get()) != 0) {
            throw std::runtime_error("Failed to initialize encryption stream");
        }

        EncryptingWriter writer(out, context.release());
        writer.writeSalt(salt.get());
        writer.writeHeader(header.get());
        for (auto filePath: filesPaths) {
            writer.writeFile(rootPath, filePath, bufferSize);
        }
        writer.addEndTag();

        out.close();
    }

    std::unique_ptr<const unsigned char[]> EncryptionController::generateSalt() {
        std::unique_ptr<unsigned char[]> salt = std::make_unique<unsigned char[]>(Chunk::SALT_LENGTH);
        randombytes_buf(salt.get(), Chunk::SALT_LENGTH);
        return salt;
    }
}