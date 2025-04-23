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
            const std::u8string outputFilename,
            const std::filesystem::path rootPath,
            const std::vector<std::filesystem::path>& filesPaths
        );

    private:
        static std::shared_ptr<const unsigned char[]> generateSalt();
    };
}