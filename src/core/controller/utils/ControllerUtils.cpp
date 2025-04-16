#include "ControllerUtils.h"

namespace fe {
    std::shared_ptr<const unsigned char[]> ControllerUtils::getKeyAndDestroyPassword(
        std::array<char, 256>& password,
        const unsigned char* salt
    ) {
        auto key = std::shared_ptr<unsigned char[]>(
            new unsigned char[KEY_LENGTH],
            std::default_delete<unsigned char[]>()
        );

        if (
            crypto_pwhash(
                key.get(), KEY_LENGTH,
                password.data(), password.size(),
                salt,
                crypto_pwhash_OPSLIMIT_INTERACTIVE,
                crypto_pwhash_MEMLIMIT_INTERACTIVE,
                crypto_pwhash_ALG_DEFAULT
            ) != 0
        ) {
            throw std::runtime_error("Failed to init key");
        }
    
        sodium_memzero(password.data(), password.size());

        return std::static_pointer_cast<const unsigned char[]>(key);
    }
}