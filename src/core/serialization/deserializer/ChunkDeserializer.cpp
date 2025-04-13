#include "ChunkDeserializer.h"

#include <vector>
#include <cstring>

#include "../../enctyption/decryptor/Decryptor.h"

namespace fe {
    inline Chunk::Tag ChunkDeserializer::deserializeTag(const unsigned char* serializedTag) {
        Chunk::Tag tag;
        std::memcpy(&tag, serializedTag, Chunk::TAG_SIZE);
        return tag;
    }

    std::size_t ChunkDeserializer::deserializeSize(const unsigned char* serializedSize) {
        DecryptorResult result = decryptor.decrypt(serializedSize, ENCRYPTED_SIZE_SIZE);

        std::size_t size;
        std::memcpy(&size, result.data.get(), Chunk::SIZE_SIZE);
        return size;
    }

    Chunk ChunkDeserializer::deserializeChunk(Chunk::Tag tag, std::size_t size, const unsigned char* serializedContent) {
        std::size_t encryptedSize = size + Decryptor::ENCRYPTION_OVERHEAD;
        DecryptorResult result = decryptor.decrypt(serializedContent, encryptedSize);

        return Chunk(tag, size, result.data.get());
    }
}
