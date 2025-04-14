#include "DecryptionController.h"

namespace fe {
    void DecryptionController::decrypt(
        const std::filesystem::path outputPath,
        const std::filesystem::path& decryptedFilePath,
        std::array<char, 256>& password
    ) {
        std::ifstream in(decryptedFilePath, std::ios::binary);
        if (!in) {
            throw std::runtime_error("Failed to open decryped file");
        }

        DecryptingReader reader(in);
        Chunk currentChunk = reader.readNextChunk();
        while (currentChunk.tag() != Chunk::Tag::FE_END_OF_STREAM) {
            switch (currentChunk.tag())
            {
            case Chunk::Tag::FE_SALT:
                initReaderContext(currentChunk, password, reader);
                break;
            case Chunk::Tag::FE_FILE_BEGIN:
                recreateFile(currentChunk, outputPath, reader);
                break;
            default:
                break;
            }

            currentChunk = reader.readNextChunk();
        }

        in.close();
    }

    void DecryptionController::initReaderContext(Chunk &saltChunk, std::array<char, 256>& password, DecryptingReader &reader) {
        Chunk headerChunk = reader.readNextChunk();
        if (headerChunk.tag() != Chunk::Tag::FE_HEADER) {
            throw std::runtime_error("Data integrity has been compromised");
        }

        auto key = ControllerUtils::getKeyAndDestroyPassword(password, saltChunk.data());

        auto context = std::make_unique<SecretStreamContext>();
        if (crypto_secretstream_xchacha20poly1305_init_pull(context.get(), headerChunk.data(), key.get()) != 0) {
            throw std::runtime_error("Data integrity has been compromised");
        }
        reader.setContext(context.release());
    }

    void DecryptionController::recreateFile(Chunk& pathChunk, const std::filesystem::path& outputPath, DecryptingReader &reader) {
        std::string pathStr(reinterpret_cast<const char*>(pathChunk.data()), pathChunk.size());
        std::filesystem::path outputFilePath = outputPath / std::filesystem::u8path(pathStr);
        
        std::filesystem::create_directories(outputFilePath.parent_path());
        std::ofstream out(outputFilePath, std::ios::binary);

        Chunk currentChunk = reader.readNextChunk();
        while (currentChunk.tag() != Chunk::Tag::FE_END_OF_FILE) {
            if (currentChunk.tag() != Chunk::Tag::FE_FILE_CONTENT_BLOCK) {
                throw std::runtime_error("Data integrity has been compromised");
            }

            out.write(reinterpret_cast<const char*>(currentChunk.data()), static_cast<std::streamsize>(currentChunk.size()));
            currentChunk = reader.readNextChunk();
        }

        out.close();
    }
}