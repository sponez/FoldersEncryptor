namespace fe {
    template <typename T>
    inline std::enable_if_t<std::is_trivially_copyable_v<T>, EncryptorResult>
    Encryptor::encrypt(const T* data, const std::size_t& size) {
        auto cipher = std::make_unique<unsigned char[]>(size + ENCRYPTION_OVERHEAD);
        unsigned long long encryptedLen;

        if (
            crypto_secretstream_xchacha20poly1305_push(
                context,
                cipher.get(), &encryptedLen,
                reinterpret_cast<const unsigned char*>(data), size,
                nullptr, 0, 0
            ) != 0
        ) {
            throw std::runtime_error("Failed to encrypt data");
        }

        return EncryptorResult(std::move(cipher), encryptedLen);
    }
}