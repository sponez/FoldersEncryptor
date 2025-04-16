#pragma once

#include <string>
#include <memory>

#include <sodium.h>

#include "../../model/chunk/Chunk.h"

namespace fe {
    class EncryptionUtils {
        public:
            static const std::size_t NONCE_SIZE = crypto_aead_xchacha20poly1305_ietf_NPUBBYTES;

            static std::shared_ptr<const unsigned char[]> getNonce(const std::shared_ptr<const unsigned char[]>& salt, const std::size_t index);
    };
}