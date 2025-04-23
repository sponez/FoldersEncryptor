#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>

#include "../../../application/ApplicationRegistry.hpp"

namespace fe {
    struct EncryptionInfo {
        bool main;
        bool filePasswordFlag;
        bool usbFlag;

        uint8_t toByte() const {
            uint8_t result = 0;
            result |= main             ? (1 << 0) : 0;
            result |= filePasswordFlag ? (1 << 1) : 0;
            result |= usbFlag          ? (1 << 2) : 0;
            return result;
        }

        static EncryptionInfo fromByte(uint8_t byte) {
            EncryptionInfo info;
            info.main             = byte & (1 << 0);
            info.filePasswordFlag = byte & (1 << 1);
            info.usbFlag          = byte & (1 << 2);
            return info;
        }
    };
    
    EncryptionInfo pullEncryptionInfo() {
        auto file = *ApplicationRegistry::pull<std::filesystem::path>(ApplicationRegistry::Key::FILE_TO_DECRYPT);
        auto inputStream = std::ifstream(file, std::ios::binary);

        uint8_t byte = 0;
        inputStream.read(reinterpret_cast<char*>(&byte), 1);
        inputStream.close();

        return EncryptionInfo::fromByte(byte);
    }
}