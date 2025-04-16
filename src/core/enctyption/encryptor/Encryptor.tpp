namespace fe {
    template <typename T>
    inline std::enable_if_t<std::is_trivially_copyable_v<T>, EncryptorResult>
    Encryptor::encrypt(const T* data, const std::size_t& size) {
        auto cipher = std::make_unique<unsigned char[]>(size + ENCRYPTION_OVERHEAD);
        unsigned long long encryptedLen;
        auto nonce = EncryptionUtils::getNonce(salt, index.load());

        if (
            crypto_aead_xchacha20poly1305_ietf_encrypt(
                cipher.get(), &encryptedLen,
                reinterpret_cast<const unsigned char*>(data), size,
                nullptr, 0,
                nullptr,
                nonce.get(),
                key.get()
            ) != 0
        ) {
            throw std::runtime_error("Failed to encrypt data");
        }

        return EncryptorResult(std::move(cipher), encryptedLen);
    }
}