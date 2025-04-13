#pragma once

#include <array>
#include <filesystem>
#include <memory>
#include <vector>

#include <sodium.h>
#include "../../model/chunk/Chunk.h"
#include "../../io/writer/EncryptingWriter.h"
#include "../../../utils/files/FilesUtils.h"

namespace fe {
    class EncryptionController {
    public:
        inline static const unsigned long long KEY_LENGTH = crypto_secretstream_xchacha20poly1305_KEYBYTES;
        static void encrypt(
            const std::string& outputFilename,
            const std::vector<std::filesystem::path>& filesPaths,
            std::array<char, 256>& password,
            const std::size_t& bufferSize
        );

    private:
        static std::unique_ptr<const unsigned char[]> generateSalt();
        static std::unique_ptr<const unsigned char[]> getKeyAndDestroyPassword(
            std::array<char, 256>& password,
            const std::unique_ptr<const unsigned char[]>& salt
        );
    };
}