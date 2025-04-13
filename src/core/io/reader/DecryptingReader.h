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
            DecryptingReader(std::istream& stream): inStream(stream), chunkDeserializer() {};
            ~DecryptingReader() = default;

            void setContext(SecretStreamContext* context) {
                chunkDeserializer = ChunkDeserializer(Decryptor(context));
            }

            Chunk readNextChunk();
            
        private:
            std::istream& inStream;
            ChunkDeserializer chunkDeserializer;
    };
}