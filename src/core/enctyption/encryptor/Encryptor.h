#pragma once

#include <stdexcept>
#include <type_traits>
#include <atomic>
#include <sodium.h>

#include "EncryptorResult.h"
#include "../utils/EncryptionUtils.h"

namespace fe {
    typedef crypto_secretstream_xchacha20poly1305_state SecretStreamContext;

    class Encryptor {
    public:
        inline static const uint8_t ENCRYPTION_OVERHEAD = crypto_aead_xchacha20poly1305_ietf_ABYTES;

        Encryptor() {}
        Encryptor(const Encryptor& other): Encryptor(other.key, other.salt) {}
        Encryptor(std::shared_ptr<const unsigned char[]> key, std::shared_ptr<const unsigned char[]> salt):
            key(std::move(key)), salt(std::move(salt)) {}

        template <typename T>
        [[nodiscard]] std::enable_if_t<std::is_trivially_copyable_v<T>, EncryptorResult>
        encrypt(const T* data, const std::size_t& size);
        
    private:
        std::atomic<std::size_t> index{0};
        std::shared_ptr<const unsigned char[]> key;
        std::shared_ptr<const unsigned char[]> salt;
    };
}

#include "Encryptor.tpp"