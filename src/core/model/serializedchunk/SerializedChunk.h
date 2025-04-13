#pragma once

#include <vector>
#include <memory>
#include <cstddef>
#include <stdexcept>
#include <algorithm>

namespace fe {
    class SerializedChunk {
    public:
        SerializedChunk(std::vector<unsigned char>&& buffer)
            : _size(buffer.size()), _data(std::make_unique<unsigned char[]>(buffer.size())) {
            std::copy(buffer.begin(), buffer.end(), _data.get());
        }

        [[nodiscard]] std::size_t size() const noexcept { return _size; }
        [[nodiscard]] const unsigned char* data() const noexcept { return _data.get(); }
        
    private:
        std::unique_ptr<unsigned char[]> _data;
        std::size_t _size;
    };
}