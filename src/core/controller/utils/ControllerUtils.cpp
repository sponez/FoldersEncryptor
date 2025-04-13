#include "ControllerUtils.h"

namespace fe {
    std::unique_ptr<const unsigned char[]> ControllerUtils::getKeyAndDestroyPassword(
        std::array<char, 256>& password,
        const unsigned char* salt
    ) {
        std::unique_ptr<unsigned char[]> key = std::make_unique<unsigned char[]>(KEY_LENGTH);

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
    
        std::fill(const_cast<char*>(password.data()), const_cast<char*>(password.data()) + password.size(), '\0');

        return key;
    }
}