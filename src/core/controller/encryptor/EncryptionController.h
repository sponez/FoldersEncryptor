#pragma once

#include <array>
#include <filesystem>
#include <vector>

#include <sodium.h>

#include "../utils/ControllerUtils.h"
#include "../../model/chunk/Chunk.h"
#include "../../io/writer/EncryptingWriter.h"
#include "../../../utils/files/FilesUtils.h"

namespace fe {
    class EncryptionController {
    public:        
        static void encrypt(
            const std::string outputFilename,
            const std::filesystem::path rootPath,
            const std::vector<std::filesystem::path>& filesPaths,
            std::array<char, 256>& password,
            const std::size_t& bufferSize,
            const std::size_t& threadCount,
            std::atomic<std::size_t>* bytesProcessed
        );

    private:
        static std::shared_ptr<const unsigned char[]> generateSalt();
    };
}