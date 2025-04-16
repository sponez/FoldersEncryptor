#include "EncryptionUtils.h"

namespace fe {
    std::shared_ptr<const unsigned char[]> EncryptionUtils::getNonce(const std::shared_ptr<const unsigned char[]> &salt, const std::size_t index) {
        std::unique_ptr<unsigned char[]> rawNonce(new unsigned char[NONCE_SIZE]);

        crypto_generichash(
            rawNonce.get(), NONCE_SIZE,
            salt.get(), Chunk::SALT_SIZE,
            reinterpret_cast<const unsigned char*>(&index), Chunk::SIZE_SIZE
        );

        std::shared_ptr<unsigned char[]> sharedRawNonce(std::move(rawNonce));

        return std::static_pointer_cast<const unsigned char[]>(sharedRawNonce);
    }
}