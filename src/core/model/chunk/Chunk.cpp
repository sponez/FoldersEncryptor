#include "Chunk.h"

namespace fe {
    Chunk Chunk::salt(const unsigned char* salt) {
        return Chunk(Tag::SALT, SALT_LENGTH, salt);
    }

    Chunk Chunk::header(const unsigned char* header) {
        return Chunk(Tag::HEADER, HEADER_LENGTH, header);
    }

    Chunk Chunk::filePath(std::filesystem::path path) {
        return Chunk(
            Tag::FILE_BEGIN,
            path.generic_u8string().length(),
            reinterpret_cast<const unsigned char*>(path.generic_u8string().data())
        );
    }

    Chunk Chunk::fileBlock(std::vector<unsigned char> block) {
        return Chunk(Tag::FILE_CONTENT_BLOCK, block);
    }

    const Chunk Chunk::END_OF_FILE = Chunk(Tag::END_OF_FILE, 0, nullptr);
    
    const Chunk Chunk::END_OF_STREAM = Chunk(Tag::END_OF_STREAM, 0, nullptr);
}