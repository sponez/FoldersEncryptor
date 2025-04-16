#pragma once

#include <type_traits>
#include <atomic>
#include <sodium.h>

#include "DecryptorResult.h"
#include "../utils/EncryptionUtils.h"

namespace fe {
    typedef crypto_secretstream_xchacha20poly1305_state SecretStreamContext;

    class Decryptor {
    public:
        inline static const uint8_t ENCRYPTION_OVERHEAD = crypto_aead_xchacha20poly1305_ietf_ABYTES;

        Decryptor() {}
        Decryptor(const Decryptor& other): Decryptor(other.key, other.salt) {}
        Decryptor(std::shared_ptr<const unsigned char[]> key, std::shared_ptr<const unsigned char[]> salt):
            key(std::move(key)), salt(std::move(salt)) {}

        DecryptorResult decrypt(const unsigned char* data, unsigned long long size);
        
    private:
        std::atomic<std::size_t> index{0};
        std::shared_ptr<const unsigned char[]> key;
        std::shared_ptr<const unsigned char[]> salt;
    };
}