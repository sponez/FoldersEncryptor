#pragma once

#include <memory>
#include <stdlib.h>

namespace fe {
    struct EncryptorResult {        
        std::unique_ptr<unsigned char[]> data;
        unsigned long long size;

        [[nodiscard]] EncryptorResult(std::unique_ptr<unsigned char[]> data, unsigned long long size)
            : data(std::move(data)), size(size) {}
    };
}