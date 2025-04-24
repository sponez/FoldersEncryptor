#include "ControllerUtils.h"

namespace fe {
    std::shared_ptr<const unsigned char[]> ControllerUtils::getKey(
        const std::optional<std::u8string>& usernameHash,
        const std::optional<std::u8string>& passwordHash,
        const std::optional<std::u8string>& filePasswordHash,
        const std::optional<std::u8string>& usbIdHash,
        const unsigned char* salt
    ) {
        crypto_generichash_state state;
        crypto_generichash_init(&state, nullptr, 0, KEY_LENGTH);
    
        crypto_generichash_update(&state, salt, Chunk::SALT_SIZE);
    
        if (usernameHash) {
            std::string usernameHashStr = StringUtils::string(*usernameHash);
            crypto_generichash_update(&state, reinterpret_cast<const unsigned char*>(usernameHashStr.data()), usernameHashStr.size());
        }
        if (passwordHash) {
            std::string passwordHashStr = StringUtils::string(*passwordHash);
            crypto_generichash_update(&state, reinterpret_cast<const unsigned char*>(passwordHashStr.data()), passwordHashStr.size());
        }
        if (filePasswordHash) {
            std::string filePasswordHashStr = StringUtils::string(*filePasswordHash);
            crypto_generichash_update(&state, reinterpret_cast<const unsigned char*>(filePasswordHashStr.data()), filePasswordHashStr.size());
        }
        if (usbIdHash) {
            std::string usbIdHashStr = StringUtils::string(*usbIdHash);
            crypto_generichash_update(&state, reinterpret_cast<const unsigned char*>(usbIdHashStr.data()), usbIdHashStr.size());
        }
    
        auto key = std::shared_ptr<unsigned char[]>(
            new unsigned char[KEY_LENGTH],
            std::default_delete<unsigned char[]>()
        );
        crypto_generichash_final(&state, key.get(), KEY_LENGTH);
    
        return std::static_pointer_cast<const unsigned char[]>(key);
    }
}