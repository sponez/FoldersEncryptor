#pragma once

#include "../../model/chunk/Chunk.h"
#include "../../model/serializedchunk/SerializedChunk.h"
#include "../../enctyption/decryptor/Decryptor.h"

namespace fe {
    class ChunkDeserializer {
    public:
        inline static unsigned long long ENCRYPTED_SIZE_SIZE = Chunk::SIZE_SIZE + Decryptor::ENCRYPTION_OVERHEAD;

        static Chunk::Tag deserializeTag(const unsigned char* serializedTag);

        ChunkDeserializer(Decryptor& decryptor): decryptor(decryptor) {}

        std::size_t deserializeSize(const unsigned char* serializedSize);
        Chunk deserializeChunk(Chunk::Tag tag, std::size_t size, const unsigned char* serializedContent);

    private:
        Decryptor decryptor;
    };
}