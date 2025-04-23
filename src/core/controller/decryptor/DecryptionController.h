#pragma once

#include <filesystem>
#include <fstream>

#include "../utils/ControllerUtils.h"
#include "../../model/chunk/Chunk.h"
#include "../../io/reader/DecryptingReader.h"

namespace fe {
    class DecryptionController {
        public:
            static void decrypt(
                std::filesystem::path outputPath,
                std::filesystem::path decryptedFilePath
            );
        
        private:
            static void initReaderContext(
                Chunk& saltChunk,
                const std::optional<std::u8string>& usernameHash,
                const std::optional<std::u8string>& passwordHash,
                const std::optional<std::u8string>& filePasswordHash,
                const std::optional<std::u8string>& usbIdHash,
                DecryptingReader& reader
            );
            
            static void recreateFile(
                Chunk& pathChunk,
                const std::filesystem::path& outputPath,
                DecryptingReader& reader
            );
    };
}