#pragma once

#include <filesystem>
#include <fstream>
#include <thread>
#include <vector>

#include "../../model/chunk/Chunk.h"
#include "../../serialization/serializer/ChunkSerializer.h"
#include "../../enctyption/encryptor/Encryptor.h"
#include "../../multithreading/BlockingStrictIndexedQueue.hpp"

namespace fe {
    class EncryptingWriter {
    public:
        EncryptingWriter(
            std::ostream& stream,
            std::shared_ptr<const unsigned char[]> key,
            std::shared_ptr<const unsigned char[]> salt,
            const std::size_t& threadCount,
            std::atomic<std::size_t>* bytesProcessed
        ): outStream(stream), chunkSerializer(Encryptor(key, salt)), bytesProcessed(bytesProcessed) {
            this->threadCount = threadCount;
            startSerializerThreads();
            startWriterThread();
        }

        ~EncryptingWriter() = default;

        void writeSalt(const unsigned char* salt);
        void writeFile(
            const std::filesystem::path rootPath,
            const std::filesystem::path& filePath,
            const std::size_t& bufferSize
        );
        void addEndTag();
        void close();

    private:
        std::atomic<std::size_t>* bytesProcessed;

        std::ostream& outStream;
        ChunkSerializer chunkSerializer;

        std::size_t index = 0;
        BlockingStrictIndexedQueue<Chunk> queueToEncrypt;
        BlockingStrictIndexedQueue<SerializedChunk> queueToWrite;
    
        std::vector<std::thread> serializerThreads;
        std::thread writerThread;
        std::size_t threadCount;
        std::mutex _mutex;

        std::mutex workerMutex;
        std::atomic<std::size_t> activeSerialaizers{0};
        std::atomic<std::size_t> activeWriters{0};
        std::condition_variable workersFinished;

        void writeFilePath(const std::filesystem::path& filePath, const std::filesystem::path& rootPath);
        void writeFileContent(std::ifstream& in, const std::size_t& bufferSize);
        void writeFileEnd();
        void synchronizedWrite(SerializedChunk& serializedChunk);

        void startSerializerThreads();
        void startWriterThread();
    };
}