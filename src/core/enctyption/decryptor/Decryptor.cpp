#include "Decryptor.h"

#include <stdexcept>

namespace fe {
    DecryptorResult Decryptor::decrypt(const unsigned char* data, unsigned long long size) {
        auto decipher = std::make_unique<unsigned char[]>(size);
        unsigned long long decryptedLen;
        unsigned char tag = 0;

        if (
            crypto_secretstream_xchacha20poly1305_pull(
                &context,
                decipher.get(), &decryptedLen,
                &tag,
                data, size,
                nullptr, 0
            ) != 0
        ) {
            throw std::runtime_error("Failed to decrypt data");
        }

        return DecryptorResult(std::move(decipher), decryptedLen);
    }
}