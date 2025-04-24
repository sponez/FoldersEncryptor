#include "EncryptingWriter.h"

#include "../../enctyption/utils/EncryptionInfo.hpp"
#include "../../../application/ApplicationRegistry.hpp"

namespace fe {
    void EncryptingWriter::writeEncryptionInfo() {
        EncryptionInfo info;
        info.authorization = *ApplicationRegistry::pull<bool>(ApplicationRegistry::Key::AUTHORIZATION_OK);
        info.filePasswordFlag = ApplicationRegistry::containsAny(ApplicationRegistry::Key::FILE_PASSWORD);
        info.usbFlag = ApplicationRegistry::containsAny(ApplicationRegistry::Key::USB_ID);
        
        uint8_t flags = info.toByte();
        outStream.write(reinterpret_cast<const char*>(&flags), 1);
    }

    void EncryptingWriter::writeSalt(const unsigned char* salt) {
        Chunk saltChunk = Chunk::salt(salt);
        queueToEncrypt.push(saltChunk, index);
        index++;
    }

    void EncryptingWriter::writeFilePath(const std::filesystem::path& filePath, const std::filesystem::path& rootPath) {
        std::filesystem::path relativeFilePath = std::filesystem::relative(filePath, rootPath);
        Chunk filePathChunk = Chunk::filePath(relativeFilePath);
        queueToEncrypt.push(filePathChunk, index);
        index++;
    }

    void EncryptingWriter::writeFileContent(std::ifstream& in, const std::size_t& bufferSize) {
        while (in.peek() != EOF) {
            std::vector<unsigned char> plain(bufferSize);

            in.read(reinterpret_cast<char*>(plain.data()), bufferSize);
            std::streamsize readLen = in.gcount();
            if (readLen <= 0) {
                break;
            } else if (readLen > bufferSize) {
                throw std::runtime_error("Read more bytes than expected — potential overflow");
            }

            plain.resize(readLen);
            Chunk contentBlock = Chunk::fileBlock(plain);
            queueToEncrypt.push(contentBlock, index);
            index++;
        }
    }

    void EncryptingWriter::writeFileEnd() {
        Chunk fileEnd = Chunk::FE_END_OF_FILE;
        queueToEncrypt.push(fileEnd, index);
        index++;
    }

    void EncryptingWriter::writeFile(
        const std::filesystem::path rootPath,
        const std::filesystem::path& filePath,
        const std::size_t& bufferSize
    ) {
        std::ifstream in(filePath, std::ios::binary);
        if (!in) {
            throw std::runtime_error("Error occurs while opening file");
        }

        writeFilePath(filePath, rootPath);
        writeFileContent(in, bufferSize);
        writeFileEnd();

        in.close();
    }

    void EncryptingWriter::addEndTag() {
        Chunk streamEnd = Chunk::FE_END_OF_STREAM;
        queueToEncrypt.push(streamEnd, index);
        index++;
    }

    void EncryptingWriter::synchronizedWrite(SerializedChunk& serializedChunk) {
        std::lock_guard<std::mutex> lock(_mutex);
        outStream.write(reinterpret_cast<const char*>(serializedChunk.data()), serializedChunk.size());
    }

    void EncryptingWriter::startSerializerThreads() {
        for (std::size_t i = 0; i < threadCount; ++i) {
            serializerThreads.emplace_back([this]() {
                {
                    std::lock_guard<std::mutex> lock(workerMutex);
                    activeSerialaizers++;
                }

                Chunk chunk;
                while (auto index = queueToEncrypt.pop(chunk)) {
                    SerializedChunk serialized = chunkSerializer.serialize(chunk);
                    queueToWrite.push(std::move(serialized), index.value());
                }

                {
                    std::lock_guard<std::mutex> lock(workerMutex);
                    activeSerialaizers--;
                    workersFinished.notify_one();
                }
            });
        }
    }

    void EncryptingWriter::startWriterThread() {
        writerThread = std::thread([this]() {
            {
                std::lock_guard<std::mutex> lock(workerMutex);
                activeWriters++;
            }

            SerializedChunk serialized;
            while (queueToWrite.pop(serialized)) {
                synchronizedWrite(serialized);

                auto x = *ApplicationRegistry::pull<std::size_t>(ApplicationRegistry::Key::PROCESSED);
                ApplicationRegistry::push(ApplicationRegistry::Key::PROCESSED, x + serialized.size());
            }

            {
                std::lock_guard<std::mutex> lock(workerMutex);
                activeWriters--;
                workersFinished.notify_one();
            }
        });
    }
    
    void EncryptingWriter::close() {
        queueToEncrypt.close();
        {
            std::unique_lock<std::mutex> lock(workerMutex);
            workersFinished.wait(lock, [&]() {
                return activeSerialaizers.load() == 0;
            });
        }
        for (auto& t : serializerThreads) t.join();

        queueToWrite.close();
        {
            std::unique_lock<std::mutex> lock(workerMutex);
            workersFinished.wait(lock, [&]() {
                return activeWriters.load() == 0;
            });
        }
        writerThread.join();
    }
}