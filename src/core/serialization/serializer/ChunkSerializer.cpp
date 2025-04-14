#include "ChunkSerializer.h"

#include <vector>

#include "../../enctyption/encryptor/Encryptor.h"

namespace fe {
    SerializedChunk ChunkSerializer::serialize(Chunk& chunk) {
        std::vector<unsigned char> buffer;

        auto tagByte = static_cast<unsigned char>(static_cast<std::underlying_type_t<Chunk::Tag>>(chunk.tag()));
        buffer.insert(buffer.end(), &tagByte, &tagByte + Chunk::TAG_SIZE);

        switch (chunk.tag()) {
            case Chunk::Tag::FE_SALT:
            case Chunk::Tag::FE_HEADER: {
                buffer.insert(buffer.end(), chunk.data(), chunk.data() + chunk.size());
                break;
            }
            case Chunk::Tag::FE_FILE_BEGIN:
            case Chunk::Tag::FE_FILE_CONTENT_BLOCK: {
                std::size_t size = chunk.size();
                EncryptorResult sizeResult = encryptor.encrypt(&size, sizeof(size));
                buffer.insert(buffer.end(), sizeResult.data.get(), sizeResult.data.get() + sizeResult.size);

                EncryptorResult contentResult = encryptor.encrypt(chunk.data(), chunk.size());
                buffer.insert(buffer.end(), contentResult.data.get(), contentResult.data.get() + contentResult.size);
                break;
            }
            case Chunk::Tag::FE_END_OF_FILE:
            case Chunk::Tag::FE_END_OF_STREAM:
            default:
                break;
        }

        return SerializedChunk(std::move(buffer));
    }
}
