#pragma once

#include <array>
#include <memory>
#include <optional>
#include <stdexcept>
#include <sodium.h>

#include "../../model/chunk/Chunk.h"
#include "../../../application/Application.hpp"
#include "../../../application/ApplicationRegistry.hpp"

namespace fe {
    class ControllerUtils {
        public:
            inline static const unsigned long long KEY_LENGTH = crypto_secretstream_xchacha20poly1305_KEYBYTES;

            static std::shared_ptr<const unsigned char[]> getKey(
                const std::optional<std::u8string>& usernameHash,
                const std::optional<std::u8string>& passwordHash,
                const std::optional<std::u8string>& filePasswordHash,
                const std::optional<std::u8string>& usbIdHash,
                const unsigned char* salt
            );
    };
}