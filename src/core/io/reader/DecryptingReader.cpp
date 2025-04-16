#include "DecryptingReader.h"

#include <cstring>

namespace fe {
    Chunk DecryptingReader::readSalt() {
        auto tag = readTag();
        if (tag != Chunk::Tag::FE_SALT) {
            throw std::runtime_error("The chunk read is not salt");
        }

        auto saltByte = std::make_unique<const char[]>(Chunk::SALT_SIZE);
        inStream.read(const_cast<char*>(saltByte.get()), Chunk::SALT_SIZE);
        return Chunk::salt(reinterpret_cast<const unsigned char*>(saltByte.get()));
    }

    Chunk DecryptingReader::readNextFileChunk() {
        Chunk chunk;
        queueToWrite.pop(chunk);
        return chunk;
    }

    void DecryptingReader::close() {
        readerThread.join();

        queueToWrite.close();
        for (auto& t: deserializerThreads) t.join();
    }

    Chunk::Tag DecryptingReader::readTag() {
        auto tagByte = std::make_unique<const char[]>(Chunk::TAG_SIZE);
        Chunk::Tag tag;
        inStream.read(const_cast<char*>(tagByte.get()), Chunk::TAG_SIZE);
        tag = ChunkDeserializer::deserializeTag(reinterpret_cast<const unsigned char*>(tagByte.get()));
        return tag;
    }

    std::size_t DecryptingReader::readSize() {
        auto sizeByte = std::make_unique<const char[]>(ChunkDeserializer::ENCRYPTED_SIZE_SIZE);
        std::size_t size;
        inStream.read(const_cast<char*>(sizeByte.get()), ChunkDeserializer::ENCRYPTED_SIZE_SIZE);
        size = chunkDeserializer->deserializeSize(reinterpret_cast<const unsigned char*>(sizeByte.get()));
        return size;
    }

    void DecryptingReader::startReaderThread() {
        readerThread = std::thread([this]() {
            SerializedChunk serialized;
            while (synchronizedReadNextFileBlock()) {}
            queueToDecrypt.close();
        });
    }

    void DecryptingReader::startDeserializerThreads() {
        for (std::size_t i = 0; i < threadCount; ++i) {
            deserializerThreads.emplace_back([this]() {
                {
                    std::lock_guard<std::mutex> lock(workerMutex);
                    activeDeserialaizers++;
                }

                TempChunk tempChunk;
                while (auto index = queueToDecrypt.pop(tempChunk)) {
                    if (tempChunk.tag == Chunk::Tag::FE_END_OF_FILE) {
                        Chunk chunk = Chunk::FE_END_OF_FILE;
                        queueToWrite.push(std::move(chunk), index.value());   
                    } else if (tempChunk.tag == Chunk::Tag::FE_END_OF_STREAM) {
                        Chunk chunk = Chunk::FE_END_OF_STREAM;
                        queueToWrite.push(std::move(chunk), index.value());   
                    } else {
                        Chunk chunk = chunkDeserializer->deserializeChunk(tempChunk.tag, tempChunk.decryptedSize, tempChunk.encryptedData.get());
                        queueToWrite.push(std::move(chunk), index.value());   
                    }
                }

                {
                    std::lock_guard<std::mutex> lock(workerMutex);
                    activeDeserialaizers--;
                    workersFinished.notify_one();
                }
            });
        }
    }

    bool DecryptingReader::synchronizedReadNextFileBlock() {
        std::lock_guard<std::mutex> lock(_mutex);

        std::size_t size;
        std::unique_ptr<char[]> blockByte;

        auto tag = readTag();
        if (tag == Chunk::Tag::FE_SALT || tag == Chunk::Tag::FE_NULL_CHUNK) {
            throw std::runtime_error("Illegal chunk");
        }
        if (tag == Chunk::Tag::FE_END_OF_FILE || tag == Chunk::Tag::FE_END_OF_STREAM) {
            auto tempChunk = TempChunk(tag, 0, std::unique_ptr<const unsigned char[]>());
            queueToDecrypt.push(std::move(tempChunk), index);
            index++;
            return tag == Chunk::Tag::FE_END_OF_FILE;
        }

        size = readSize();
        blockByte = std::make_unique<char[]>(size + Decryptor::ENCRYPTION_OVERHEAD);
        inStream.read(blockByte.get(), size + Decryptor::ENCRYPTION_OVERHEAD);

        std::unique_ptr<const unsigned char[]> encryptedData(new unsigned char[size + Decryptor::ENCRYPTION_OVERHEAD]);
        std::memcpy(
            const_cast<unsigned char*>(encryptedData.get()),
            reinterpret_cast<unsigned char*>(blockByte.get()),
            size + Decryptor::ENCRYPTION_OVERHEAD
        );

        auto tempChunk = TempChunk(tag, size, std::move(encryptedData));
        queueToDecrypt.push(std::move(tempChunk), index);
        index++;

        return true;
    }
}