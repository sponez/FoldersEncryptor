#include "DecryptionController.h"

#include <cstring>

#include "../../../application/Application.hpp"
#include "../../../application/ApplicationRegistry.hpp"

namespace fe {
    void DecryptionController::decrypt(
        std::filesystem::path outputPath,
        std::filesystem::path decryptedFilePath
    ) {
        int threadCount = Application::properties.getPropertyValue<int>(Application::ApplicationProperties::THREAD_COUNT_KEY);

        std::optional<std::u8string> username;
        std::optional<std::u8string> password;
        if (*ApplicationRegistry::pull<bool>(ApplicationRegistry::Key::AUTHORIZATION_OK)) {
            username = Application::properties.getPropertyValue<std::u8string>(Application::ApplicationProperties::USER_KEY);
            password =  Application::properties.getPropertyValue<std::u8string>(Application::ApplicationProperties::PASSWORD_KEY);
        } else {
            username = std::nullopt;
            password = std::nullopt;
        }
        std::optional<std::u8string> filePassword = ApplicationRegistry::pull<std::u8string>(ApplicationRegistry::Key::FILE_PASSWORD);
        std::optional<std::u8string> usbId = ApplicationRegistry::pull<std::u8string>(ApplicationRegistry::Key::USB_ID);

        std::ifstream in(decryptedFilePath, std::ios::binary);
        if (!in) {
            return;
        }

        DecryptingReader reader(in, threadCount);

        reader.skipDecryptionInfo();
        Chunk saltChank = reader.readSalt();
        initReaderContext(
            saltChank,
            username,
            password,
            filePassword,
            usbId,
            reader
        );

        auto threadPool = ThreadPool();

        Chunk currentChunk = reader.readNextFileChunk();
        while (currentChunk.tag() != Chunk::Tag::FE_END_OF_STREAM) {
            recreateFile(currentChunk, outputPath, reader, threadPool);
            currentChunk = reader.readNextFileChunk();
        }
        
        threadPool.join();
        reader.close();
        in.close();
    }

    void DecryptionController::initReaderContext(
        Chunk &saltChunk,
        const std::optional<std::u8string>& usernameHash,
        const std::optional<std::u8string>& passwordHash,
        const std::optional<std::u8string>& filePasswordHash,
        const std::optional<std::u8string>& usbIdHash,
        DecryptingReader &reader
    ) {
        auto key = ControllerUtils::getKey(
            usernameHash,
            passwordHash,
            filePasswordHash,
            usbIdHash,
            saltChunk.data()
        );

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
        ThreadPool& threadPool
    ) {
        auto writeMutex = std::make_shared<std::mutex>();

        std::u8string pathStr = std::u8string(
            reinterpret_cast<const char*>(pathChunk.data()),
            reinterpret_cast<const char*>(pathChunk.data() + pathChunk.size())
        );
        std::filesystem::path outputFilePath = outputPath / std::filesystem::path(pathStr);
        
        std::filesystem::create_directories(outputFilePath.parent_path());

        Chunk currentChunk = reader.readNextFileChunk();

        std::size_t MaxBufferSize = 8 * 1024 * 1024;
        std::shared_ptr<std::vector<char>> currentBuffer = std::make_shared<std::vector<char>>();
        currentBuffer->reserve(MaxBufferSize);
        while (currentChunk.tag() != Chunk::Tag::FE_END_OF_FILE) {
            if (currentChunk.tag() != Chunk::Tag::FE_FILE_CONTENT_BLOCK) {
                throw std::runtime_error("Data integrity has been compromised");
            }

            currentBuffer->insert(
                currentBuffer->end(),
                reinterpret_cast<const char*>(currentChunk.data()),
                reinterpret_cast<const char*>(currentChunk.data() + currentChunk.size())
            );

            if (currentBuffer->size() >= MaxBufferSize) {
                auto bufferToWrite = currentBuffer;
                currentBuffer = std::make_shared<std::vector<char>>();
                currentBuffer->reserve(MaxBufferSize);
            
                threadPool.submit(
                    [outputFilePath, bufferToWrite, writeMutex]() {
                        std::scoped_lock lock(*writeMutex);

                        std::ofstream out(outputFilePath, std::ios::binary | std::ios::app);
                        out.write(bufferToWrite->data(), bufferToWrite->size());
                        out.close();
    
                        auto x = *ApplicationRegistry::pull<std::size_t>(ApplicationRegistry::Key::PROCESSED);
                        ApplicationRegistry::push(ApplicationRegistry::Key::PROCESSED, x + bufferToWrite->size());
                    }
                );
            }

            currentChunk = reader.readNextFileChunk();
        }

        if (!currentBuffer->empty()) {
            threadPool.submit(
                [outputFilePath, currentBuffer, writeMutex]() {
            std::scoped_lock lock(*writeMutex);

            std::ofstream out(outputFilePath, std::ios::binary | std::ios::app);
            out.write(currentBuffer->data(), currentBuffer->size());
            out.close();

            auto x = *ApplicationRegistry::pull<std::size_t>(ApplicationRegistry::Key::PROCESSED);
            ApplicationRegistry::push(ApplicationRegistry::Key::PROCESSED, x + currentBuffer->size());
                }
            );
        }
    }
}