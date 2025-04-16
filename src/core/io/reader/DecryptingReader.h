#pragma once

#include <filesystem>
#include <fstream>
#include <memory>

#include "../../enctyption/decryptor/Decryptor.h"
#include "../../model/chunk/Chunk.h"
#include "../../serialization/deserializer/ChunkDeserializer.h"

namespace fe {
    class DecryptingReader {
        public:
            DecryptingReader(std::istream& stream): inStream(stream) {};
            ~DecryptingReader() = default;

            void setContext(
                std::shared_ptr<const unsigned char[]> key,
                std::shared_ptr<const unsigned char[]> salt
            ) {
                chunkDeserializer = std::make_unique<ChunkDeserializer>(Decryptor(key, salt));
            }

            Chunk readNextChunk();
            
        private:
            std::istream& inStream;
            std::unique_ptr<ChunkDeserializer> chunkDeserializer;
    };
}