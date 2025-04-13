#pragma once

#include <type_traits>
#include <sodium.h>

#include "DecryptorResult.h"

namespace fe {
    typedef crypto_secretstream_xchacha20poly1305_state SecretStreamContext;

    class Decryptor {
    public:
        inline static const uint8_t ENCRYPTION_OVERHEAD = crypto_secretstream_xchacha20poly1305_ABYTES;

        Decryptor() {}
        Decryptor(SecretStreamContext* context): context(context) {}

        DecryptorResult decrypt(const unsigned char* data, unsigned long long size);
        
    private:
        SecretStreamContext* context;
    };
}