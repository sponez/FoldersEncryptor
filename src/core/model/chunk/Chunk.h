#pragma once

#include <vector>
#include <array>
#include <memory>
#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <filesystem>

#include <sodium.h>

namespace fe {
    class Chunk {
        public:
            enum class Tag: uint8_t;

            inline static const uint8_t SALT_LENGTH = crypto_pwhash_SALTBYTES;            
            inline static const uint8_t HEADER_LENGTH = crypto_secretstream_xchacha20poly1305_HEADERBYTES;
            inline static const uint8_t TAG_SIZE = sizeof(uint8_t);
            inline static const uint8_t SIZE_SIZE = sizeof(std::size_t);

            static Chunk salt(const unsigned char* salt);
            static Chunk header(const unsigned char* header);
            static Chunk filePath(std::filesystem::path path);
            static Chunk fileBlock(std::vector<unsigned char> block);
            static const Chunk END_OF_FILE;
            static const Chunk END_OF_STREAM;
            static const Chunk NULL_CHUNK;

            Chunk(Tag tag, std::size_t size, const unsigned char* data)
                : _tag(tag), _size(size), _content(std::make_unique<unsigned char[]>(size)) {
                std::copy(data, data + size, _content.get());
            }

            Chunk(const Chunk& other)
                : Chunk(other._tag, other._size, other._content.get()) {}

            ~Chunk() = default;

            Chunk& operator=(const Chunk& other) {
                if (this != &other) {
                    _tag = other._tag;
                    _size = other._size;
                    _content = std::make_unique<unsigned char[]>(_size);
                    std::copy(other._content.get(), other._content.get() + _size, _content.get());
                }
                return *this;
            }

            Chunk(Chunk&&) noexcept = default;
            Chunk& operator=(Chunk&&) noexcept = default;

            [[nodiscard]] Tag tag() const noexcept { return _tag; }
            [[nodiscard]] std::size_t size() const noexcept { return _size; }
            [[nodiscard]] const unsigned char* data() const noexcept { return _content.get(); }
            [[nodiscard]] std::vector<unsigned char> dataVector() const noexcept { return std::vector<unsigned char>(_content.get(), _content.get() + _size); }

        private:
            Chunk(Tag tag, const std::vector<unsigned char>& data)
                : Chunk(tag, data.size(), data.data()) {}

            template<std::size_t N>
            Chunk(Tag tag, const std::array<unsigned char, N>& data)
                : Chunk(tag, N, data.data()) {}

            Tag _tag;
            std::size_t _size;
            std::unique_ptr<unsigned char[]> _content;
    };

    enum class Chunk::Tag : uint8_t {
        SALT,
        HEADER,
        FILE_BEGIN,
        FILE_CONTENT_BLOCK,
        END_OF_FILE,
        END_OF_STREAM,
        NULL_CHUNK
    };
}