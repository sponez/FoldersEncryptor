#include "EncryptionController.h"

namespace fe {
    void EncryptionController::encrypt(
        const std::string& outputFilename,
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
        auto key = getKeyAndDestroyPassword(password, salt);
        auto header = std::make_unique<const unsigned char[]>(Chunk::HEADER_LENGTH);
        crypto_secretstream_xchacha20poly1305_state* state;
        if (crypto_secretstream_xchacha20poly1305_init_push(state, const_cast<unsigned char*>(header.get()), key.get()) != 0) {
            throw std::runtime_error("Failed to initialize encryption stream");
        }

        EncryptingWriter writer(out, state);
        writer.writeSalt(salt.get());
        writer.writeHeader(header.get());
        for (auto filePath: filesPaths) {
            writer.writeFile(filePath, bufferSize);
        }
        writer.addEndTag();

        out.close();
    }

    std::unique_ptr<const unsigned char[]> EncryptionController::generateSalt() {
        std::unique_ptr<unsigned char[]> salt = std::make_unique<unsigned char[]>(Chunk::SALT_LENGTH);
        randombytes_buf(salt.get(), Chunk::SALT_LENGTH);
        return salt;
    }

    std::unique_ptr<const unsigned char[]> EncryptionController::getKeyAndDestroyPassword(
        std::array<char, 256>& password,
        const std::unique_ptr<const unsigned char[]>& salt
    ) {
        std::unique_ptr<unsigned char[]> key = std::make_unique<unsigned char[]>(KEY_LENGTH);

        if (
            crypto_pwhash(
                key.get(), KEY_LENGTH,
                password.data(), password.size(),
                salt.get(),
                crypto_pwhash_OPSLIMIT_INTERACTIVE,
                crypto_pwhash_MEMLIMIT_INTERACTIVE,
                crypto_pwhash_ALG_DEFAULT
            ) != 0
        ) {
            throw std::runtime_error("Failed to init key");
        }
    
        std::fill(const_cast<char*>(password.data()), const_cast<char*>(password.data()) + password.size(), '\0');

        return key;
    }
}