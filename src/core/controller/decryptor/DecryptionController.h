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
                std::filesystem::path decryptedFilePath,
                std::array<char, 256>& password,
                const std::size_t& threadCount,
                std::atomic<std::size_t>* bytesProcessed
            );
        
        private:
            static void initReaderContext(Chunk& saltChunk, std::array<char, 256>& password, DecryptingReader& reader);
            static void recreateFile(
                Chunk& pathChunk,
                const std::filesystem::path& outputPath,
                DecryptingReader& reader,
                std::atomic<std::size_t>* bytesProcessed
            );
    };
}