#include "EncryptionController.h"

#include "../../../application/Application.hpp"
#include "../../../application/ApplicationRegistry.hpp"

namespace fe {
    void EncryptionController::encrypt(
        std::u8string outputFilename,
        std::filesystem::path rootPath,
        const std::vector<std::filesystem::path>& filesPaths
    ) {
        std::uint8_t threadCount = Application::properties.getPropertyValue<int>(Application::ApplicationProperties::BUFFER_SIZE_KEY);
        std::uint8_t bufferSize = Application::properties.getPropertyValue<int>(Application::ApplicationProperties::THREAD_COUNT_KEY);

        std::optional<std::u8string> username;
        std::optional<std::u8string> password;
        if (*ApplicationRegistry::pull<bool>(ApplicationRegistry::Key::AUTHORIZATION_SKIPED)) {
            username = std::nullopt;
            password = std::nullopt;
        } else {
            username = Application::properties.getPropertyValue<std::u8string>(Application::ApplicationProperties::USER_KEY);
            password =  Application::properties.getPropertyValue<std::u8string>(Application::ApplicationProperties::PASSWORD_KEY);
        }

        std::optional<std::u8string> filePassword = ApplicationRegistry::pull<std::u8string>(ApplicationRegistry::Key::FILE_PASSWORD);
        std::optional<std::u8string> usbId = ApplicationRegistry::pull<std::u8string>(ApplicationRegistry::Key::USB_ID);

        auto outputPath = rootPath / outputFilename;
        std::ofstream out(outputPath, std::ios::binary);
        if (!out.is_open()) {
            throw std::runtime_error("Failed to open output file");
        }

        auto salt = generateSalt();
        auto key =
            ControllerUtils::getKey(
                username,
                password,
                filePassword,
                usbId,
                salt.get()
            );

        EncryptingWriter writer(out, key, salt, threadCount);
        writer.writeSalt(salt.get());
        for (auto filePath: filesPaths) {
            writer.writeFile(rootPath, filePath, bufferSize);
        }
        writer.addEndTag();
        writer.close();

        out.close();
    }

    std::shared_ptr<const unsigned char[]> EncryptionController::generateSalt() {
        auto salt = std::shared_ptr<unsigned char[]>(
            new unsigned char[Chunk::SALT_SIZE],
            std::default_delete<unsigned char[]>()
        );

        randombytes_buf(salt.get(), Chunk::SALT_SIZE);

        return std::static_pointer_cast<const unsigned char[]>(salt);
    }
}