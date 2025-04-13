#pragma once

#include <stdexcept>
#include <type_traits>
#include <sodium.h>

#include "EncryptorResult.h"

namespace fe {
    typedef crypto_secretstream_xchacha20poly1305_state SecretStreamContext;

    class Encryptor {
    public:
        inline static const uint8_t ENCRYPTION_OVERHEAD = crypto_secretstream_xchacha20poly1305_ABYTES;

        Encryptor() {}
        Encryptor(SecretStreamContext* context): context(context) {}

        template <typename T>
        [[nodiscard]] std::enable_if_t<std::is_trivially_copyable_v<T>, EncryptorResult>
        encrypt(const T* data, const std::size_t& size);
        
    private:
        SecretStreamContext* context;
    };
}

#include "Encryptor.tpp"