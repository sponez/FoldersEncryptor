#include "DecryptingReader.h"

namespace fe {
    Chunk DecryptingReader::readNextChunk() {
        auto tagByte = std::make_unique<const char[]>(Chunk::TAG_SIZE);
        Chunk::Tag tag;
        inStream.read(const_cast<char*>(tagByte.get()), Chunk::TAG_SIZE);
        tag = ChunkDeserializer::deserializeTag(reinterpret_cast<const unsigned char*>(tagByte.get()));

        switch (tag)
        {
        case Chunk::Tag::SALT: {
            auto saltByte = std::make_unique<const char[]>(Chunk::SALT_LENGTH);
            inStream.read(const_cast<char*>(saltByte.get()), Chunk::SALT_LENGTH);
            return Chunk::salt(reinterpret_cast<const unsigned char*>(saltByte.get()));
        }
        case Chunk::Tag::HEADER: {
            auto headertByte = std::make_unique<const char[]>(Chunk::HEADER_LENGTH);
            inStream.read(const_cast<char*>(headertByte.get()), Chunk::HEADER_LENGTH);
            return Chunk::header(reinterpret_cast<const unsigned char*>(headertByte.get()));
        }
        case Chunk::Tag::FILE_BEGIN:
        case Chunk::Tag::FILE_CONTENT_BLOCK: {
            auto sizeByte = std::make_unique<const char[]>(ChunkDeserializer::ENCRYPTED_SIZE_SIZE);
            std::size_t size;
            inStream.read(const_cast<char*>(sizeByte.get()), ChunkDeserializer::ENCRYPTED_SIZE_SIZE);
            size = chunkDeserializer.deserializeSize(reinterpret_cast<const unsigned char*>(sizeByte.get()));

            auto blockByte = std::make_unique<const char[]>(size + Decryptor::ENCRYPTION_OVERHEAD);
            inStream.read(const_cast<char*>(blockByte.get()), size + Decryptor::ENCRYPTION_OVERHEAD);
            return chunkDeserializer.deserializeChunk(tag, size, reinterpret_cast<const unsigned char*>(blockByte.get()));
        }
        case Chunk::Tag::END_OF_FILE:
            return Chunk::END_OF_FILE;
        case Chunk::Tag::END_OF_STREAM:
            return Chunk::END_OF_STREAM;
        default:
            break;
        }

        return Chunk::NULL_CHUNK;
    }
}