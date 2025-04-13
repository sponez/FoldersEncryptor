#pragma once

#include "../../model/chunk/Chunk.h"
#include "../../model/serializedchunk/SerializedChunk.h"
#include "../../enctyption/encryptor/Encryptor.h"

namespace fe {
    class ChunkSerializer {
    public:
        ChunkSerializer() {}
        ChunkSerializer(Encryptor encryptor): encryptor(encryptor) {}

        SerializedChunk serialize(Chunk& chunk);

    private:
        Encryptor encryptor;
    };
}