#pragma once

#include <filesystem>
#include <string>
#include <thread>
#include <vector>

#include "ProgressBarGuiWindow.hpp"
#include "ProgressBarWindowAction.hpp"
#include "../GuiWindowId.hpp"
#include "../abstract/GuiWindowController.hpp"
#include "../functional/FunctionalWindowAction.hpp"
#include "../../../application/ApplicationRegistry.hpp"
#include "../../../core/processor/Processor.h"
#include "../../../utils/files/FilesUtils.h"

namespace fe {
    class ProgressBarGuiWindowController: public GuiWindowController {
        private:
            ProgressBarGuiWindowController() = default;
            ~ProgressBarGuiWindowController() = default;
            
            ProgressBarGuiWindow* window = &ProgressBarGuiWindow::getInstance();

            void findSizeToProcess(std::vector<std::filesystem::path> files) {
                for (auto file: files) {
                    ProgressBarGuiWindow::getInstance().sizeToProcess += std::filesystem::file_size(file);
                }
            }

        public:
            static ProgressBarGuiWindowController& getInstance() {
                static ProgressBarGuiWindowController instance;
                return instance;
            }

            std::optional<GuiWindowId> process() override {
                if (!ApplicationRegistry::containsAny(ApplicationRegistry::Key::PROCESSING)) {
                    auto action = *ApplicationRegistry::pull<FunctionalWindowAction>(ApplicationRegistry::Key::CURRENT_ACTION);

                    ApplicationRegistry::push(ApplicationRegistry::Key::PROCESSING, true);
                    ApplicationRegistry::push(ApplicationRegistry::Key::PROCESSED, (std::size_t)0);

                    std::thread worker(
                        [action, this]() {
                            try {
                                startAction(action);
                            } catch (const std::exception& e) {
                                ApplicationRegistry::push(ApplicationRegistry::Key::PROCESSING, false);
                                ApplicationRegistry::push(ApplicationRegistry::Key::ENCRYPTION_ERROR, std::string(e.what()));
                            }
                        }
                    );
                    worker.detach();
                }

                window->setAndDraw();

                switch (window->action) {
                    case ProgressBarWindowAction::DONE: {
                        window->action = ProgressBarWindowAction::NONE;
                        ApplicationRegistry::flush();
                        return GuiWindowId::FUNCTIONAL;
                    }

                    case ProgressBarWindowAction::DONE_WITH_ERROR:
                        window->action = ProgressBarWindowAction::NONE;
                        return GuiWindowId::ENCRYPTION_ERROR;

                    default:
                        return std::nullopt;
                }
            }

            void startAction(FunctionalWindowAction action) {
                switch (action) {
                    case FunctionalWindowAction::ENCRYPT: {
                        std::u8string outputFilename;
                        std::filesystem::path rootPath;
                        std::vector<std::filesystem::path> filesToEcnrypt;

                        if (ApplicationRegistry::containsAny(ApplicationRegistry::Key::FOLDER_TO_ENCRYPT)) {
                            auto folder = *ApplicationRegistry::pull<std::filesystem::path>(ApplicationRegistry::Key::FOLDER_TO_ENCRYPT);
                            filesToEcnrypt = FilesUtils::unpack(folder);
                            outputFilename = folder.filename().u8string() + u8".fe";
                            rootPath = std::filesystem::path(folder.parent_path());
                        } else if (ApplicationRegistry::containsAny(ApplicationRegistry::Key::FILE_TO_ENCRYPT)) {
                            auto fileToEnctypt = *ApplicationRegistry::pull<std::filesystem::path>(ApplicationRegistry::Key::FILE_TO_ENCRYPT);
                            filesToEcnrypt.push_back(fileToEnctypt);
                            outputFilename = fileToEnctypt.filename().u8string() + u8".fe";
                            rootPath = std::filesystem::path(fileToEnctypt.parent_path());
                        } else if (ApplicationRegistry::containsAny(ApplicationRegistry::Key::FILES_TO_ENCRYPT)) {
                            filesToEcnrypt = *ApplicationRegistry::pull<std::vector<std::filesystem::path>>(ApplicationRegistry::Key::FILES_TO_ENCRYPT);
                            outputFilename = *ApplicationRegistry::pull<std::u8string>(ApplicationRegistry::Key::OUPUT_FILE_NAME) + u8".fe";
                            rootPath = std::filesystem::path(filesToEcnrypt[0].parent_path());
                        } else {
                            throw std::runtime_error("Files to encrypt not found");
                        }

                        findSizeToProcess(filesToEcnrypt);

                        Processor::processEncryptOption(
                            outputFilename,
                            rootPath,
                            filesToEcnrypt
                        );

                        break;
                    }
                
                    case FunctionalWindowAction::DECTYPT: {
                        std::filesystem::path fileToDecrypt = *ApplicationRegistry::pull<std::filesystem::path>(ApplicationRegistry::Key::FILE_TO_DECRYPT);
                        std::filesystem::path outputPath = fileToDecrypt.parent_path();

                        findSizeToProcess(std::vector<std::filesystem::path>{fileToDecrypt});

                        Processor::processDecryptOption(
                            outputPath,
                            fileToDecrypt
                        );

                        break;
                    }

                    case FunctionalWindowAction::TEMPORARY_OPEN: {
                        std::filesystem::path fileToDecrypt = *ApplicationRegistry::pull<std::filesystem::path>(ApplicationRegistry::Key::FILE_TO_DECRYPT);
                        findSizeToProcess(std::vector<std::filesystem::path>{fileToDecrypt});
                        Processor::processTemporaryDecryptOption(fileToDecrypt);

                        break;
                    }

                    default:
                        break;
                }
            }
    };
}