#include "Decryptor.h"

#include <stdexcept>

namespace fe {
    DecryptorResult Decryptor::decrypt(const unsigned char* data, unsigned long long size) {
        auto decipher = std::make_unique<unsigned char[]>(size);
        unsigned long long decryptedLen;
        unsigned char tag = 0;
        auto nonce = EncryptionUtils::getNonce(salt, index.load());

        if (
            crypto_aead_xchacha20poly1305_ietf_decrypt(
                decipher.get(), &decryptedLen,
                nullptr,
                data, size,
                nullptr, 0,
                nonce.get(),
                key.get()
            ) != 0
        ) {
            throw std::runtime_error("Failed to decrypt data");
        }

        return DecryptorResult(std::move(decipher), decryptedLen);
    }
}