#include "DecryptingReader.h"

namespace fe {
    Chunk DecryptingReader::readNextChunk() {
        auto tagByte = std::make_unique<const char[]>(Chunk::TAG_SIZE);
        Chunk::Tag tag;
        inStream.read(const_cast<char*>(tagByte.get()), Chunk::TAG_SIZE);
        tag = ChunkDeserializer::deserializeTag(reinterpret_cast<const unsigned char*>(tagByte.get()));

        switch (tag)
        {
        case Chunk::Tag::FE_SALT: {
            auto saltByte = std::make_unique<const char[]>(Chunk::SALT_SIZE);
            inStream.read(const_cast<char*>(saltByte.get()), Chunk::SALT_SIZE);
            return Chunk::salt(reinterpret_cast<const unsigned char*>(saltByte.get()));
        }
        case Chunk::Tag::FE_FILE_BEGIN:
        case Chunk::Tag::FE_FILE_CONTENT_BLOCK: {
            auto sizeByte = std::make_unique<const char[]>(ChunkDeserializer::ENCRYPTED_SIZE_SIZE);
            std::size_t size;
            inStream.read(const_cast<char*>(sizeByte.get()), ChunkDeserializer::ENCRYPTED_SIZE_SIZE);
            size = chunkDeserializer->deserializeSize(reinterpret_cast<const unsigned char*>(sizeByte.get()));

            auto blockByte = std::make_unique<const char[]>(size + Decryptor::ENCRYPTION_OVERHEAD);
            inStream.read(const_cast<char*>(blockByte.get()), size + Decryptor::ENCRYPTION_OVERHEAD);
            return chunkDeserializer->deserializeChunk(tag, size, reinterpret_cast<const unsigned char*>(blockByte.get()));
        }
        case Chunk::Tag::FE_END_OF_FILE:
            return Chunk::FE_END_OF_FILE;
        case Chunk::Tag::FE_END_OF_STREAM:
            return Chunk::FE_END_OF_STREAM;
        default:
            break;
        }

        return Chunk::FE_NULL_CHUNK;
    }
}