#pragma once

#include <filesystem>
#include <fstream>

#include "../../model/chunk/Chunk.h"
#include "../../serialization/serializer/ChunkSerializer.h"
#include "../../enctyption/encryptor/Encryptor.h"

namespace fe {
    class EncryptingWriter {
    public:
        EncryptingWriter(std::ostream& stream, SecretStreamContext* context): outStream(stream) {
            chunkSerializer = ChunkSerializer(Encryptor(context));
        }
        ~EncryptingWriter() = default;

        void writeSalt(const unsigned char* salt);
        void writeHeader(const unsigned char* header);
        void writeFile(const std::filesystem::path rootPath, const std::filesystem::path& filePath, const std::size_t& bufferSize);
        void addEndTag();

    private:
        std::ostream& outStream;
        ChunkSerializer chunkSerializer;

        void serializeAndWrite(Chunk& chunk);
    };
}