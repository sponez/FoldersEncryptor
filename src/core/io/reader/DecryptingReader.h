#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include <thread>
#include <atomic>

#include "../../enctyption/decryptor/Decryptor.h"
#include "../../model/chunk/Chunk.h"
#include "../../serialization/deserializer/ChunkDeserializer.h"
#include "../../multithreading/BlockingStrictIndexedQueue.hpp"

namespace fe {
    class DecryptingReader {
        public:
            DecryptingReader(std::istream& stream, const int& threadCount): inStream(stream), threadCount(threadCount) {};
            ~DecryptingReader() = default;

            void setContext(
                std::shared_ptr<const unsigned char[]> key,
                std::shared_ptr<const unsigned char[]> salt
            ) {
                chunkDeserializer = std::make_unique<ChunkDeserializer>(Decryptor(key, salt));
                startReaderThread();
                startDeserializerThreads();
            }

            void skipDecryptionInfo();
            Chunk readSalt();
            Chunk readNextFileChunk();
            void close();
            
        private:
            struct TempChunk {
                Chunk::Tag tag;
                std::size_t decryptedSize;
                std::unique_ptr<const unsigned char[]> encryptedData;

                TempChunk() {}
                TempChunk(
                    Chunk::Tag tag,
                    std::size_t decryptedSize,
                    std::unique_ptr<const unsigned char[]> encryptedData
                ): tag(tag), decryptedSize(decryptedSize), encryptedData(std::move(encryptedData)) {}
            };
            

            std::istream& inStream;
            std::unique_ptr<ChunkDeserializer> chunkDeserializer;

            std::size_t index = 0;
            BlockingStrictIndexedQueue<TempChunk> queueToDecrypt;
            BlockingStrictIndexedQueue<Chunk> queueToWrite;

            std::vector<std::thread> deserializerThreads;
            std::thread readerThread;
            int threadCount;
            std::mutex _mutex;

            std::mutex workerMutex;
            std::atomic<int> activeDeserialaizers{0};
            std::atomic<int> activeWriters{0};
            std::condition_variable workersFinished;

            Chunk::Tag readTag();
            std::size_t readSize();

            void startReaderThread();
            void startDeserializerThreads();
            bool synchronizedReadNextFileBlock();
    };
}