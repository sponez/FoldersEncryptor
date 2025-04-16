#include "DecryptionController.h"

#include <cstring>

namespace fe {
    void DecryptionController::decrypt(
        std::filesystem::path outputPath,
        std::filesystem::path decryptedFilePath,
        std::array<char, 256>& password,
        const std::size_t& threadCount,
        std::atomic<std::size_t>* bytesProcessed
    ) {
        std::ifstream in(decryptedFilePath, std::ios::binary);
        if (!in) {
            throw std::runtime_error("Failed to open decryped file");
        }

        DecryptingReader reader(in, threadCount);

        Chunk saltChank = reader.readSalt();
        initReaderContext(saltChank, password, reader);

        Chunk currentChunk = reader.readNextFileChunk();
        while (currentChunk.tag() != Chunk::Tag::FE_END_OF_STREAM) {
            recreateFile(currentChunk, outputPath, reader, bytesProcessed);
            currentChunk = reader.readNextFileChunk();
        }

        reader.close();
        in.close();
    }

    void DecryptionController::initReaderContext(Chunk &saltChunk, std::array<char, 256>& password, DecryptingReader &reader) {
        auto key = ControllerUtils::getKeyAndDestroyPassword(password, saltChunk.data());
        auto saltCopy = std::shared_ptr<unsigned char[]>(
            new unsigned char[Chunk::SALT_SIZE],
            std::default_delete<unsigned char[]>()
        );
        std::memcpy(saltCopy.get(), saltChunk.data(), Chunk::SALT_SIZE);
        auto salt = std::static_pointer_cast<const unsigned char[]>(saltCopy);

        reader.setContext(key, salt);
    }

    void DecryptionController::recreateFile(
        Chunk& pathChunk,
        const std::filesystem::path& outputPath,
        DecryptingReader &reader,
        std::atomic<std::size_t>* bytesProcessed
    ) {
        std::string pathStr(reinterpret_cast<const char*>(pathChunk.data()), pathChunk.size());
        std::filesystem::path outputFilePath = outputPath / std::filesystem::u8path(pathStr);
        
        std::filesystem::create_directories(outputFilePath.parent_path());
        std::ofstream out(outputFilePath, std::ios::binary);

        Chunk currentChunk = reader.readNextFileChunk();
        while (currentChunk.tag() != Chunk::Tag::FE_END_OF_FILE) {
            if (currentChunk.tag() != Chunk::Tag::FE_FILE_CONTENT_BLOCK) {
                throw std::runtime_error("Data integrity has been compromised");
            }

            out.write(reinterpret_cast<const char*>(currentChunk.data()), static_cast<std::streamsize>(currentChunk.size()));
            bytesProcessed->fetch_add(currentChunk.size());
            currentChunk = reader.readNextFileChunk();
        }

        out.close();
    }
}