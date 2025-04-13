#pragma once

#include <array>
#include <memory>
#include <stdexcept>
#include <sodium.h>

namespace fe {
    class ControllerUtils {
        public:
            inline static const unsigned long long KEY_LENGTH = crypto_secretstream_xchacha20poly1305_KEYBYTES;
    
            static std::unique_ptr<const unsigned char[]> getKeyAndDestroyPassword(
                std::array<char, 256>& password,
                const unsigned char* salt
            );
    };
}