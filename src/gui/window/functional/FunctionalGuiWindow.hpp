#pragma once

#include <array>
#include <string>

#include <imgui.h>

#include "FunctionalWindowAction.hpp"
#include "../abstract/GuiWindow.hpp"
#include "../GuiUtils.hpp"
#include "../../../selector/Selector.h"
#include "../../../application/ApplicationRegistry.hpp"

namespace fe {
    class FunctionalGuiWindow: public GuiWindow {
        private:
            inline static const std::u8string ENCRYPT_BUTTON_NAME = u8"Encrypt";

            inline static const std::u8string POPUP_CHOOSE_TYPE_NAME = u8"Choose type";
            inline static const std::u8string FILE_POPUP_OPTION_NAME = u8"File(s)";
            inline static const std::u8string FOLDER_POPUP_OPTION_NAME = u8"Folder";

            inline static const std::u8string DERYPT_BUTTON_NAME = u8"Decrypt";
            inline static const std::u8string TEMPORARY_OPEN_BUTTON_NAME = u8"Temporary open";
            inline static const std::u8string PROPERTIES_BUTTON_NAME = u8"Properties";

            FunctionalGuiWindow() = default;
            ~FunctionalGuiWindow() = default;
        
        public:
            FunctionalWindowAction action = FunctionalWindowAction::NONE;
            static FunctionalGuiWindow& getInstance() {
                static FunctionalGuiWindow instance;
                return instance;
            }

        protected:
            void draw() override {
                GuiUtils::renderBackButton(
                    [&]() { action = FunctionalWindowAction::BACK; }
                );

                GuiUtils::itemGroup(
                    {
                        std::make_shared<GuiUtils::Button>(
                            ENCRYPT_BUTTON_NAME,
                            [&]() { ImGui::OpenPopup(StringUtils::string(POPUP_CHOOSE_TYPE_NAME).c_str()); }
                        ),
                        std::make_shared<GuiUtils::Button>(
                            DERYPT_BUTTON_NAME,
                            [&]() { decryptButton(); }
                        ),
                        std::make_shared<GuiUtils::Button>(
                            TEMPORARY_OPEN_BUTTON_NAME,
                            [&]() { temporaryOpenButton(); }
                        ),
                        std::make_shared<GuiUtils::Button>(
                            PROPERTIES_BUTTON_NAME,
                            [&]() { action = FunctionalWindowAction::PROPERTIES; }
                        )
                    }
                );

                fileOrFolderPopUp();
            }

        private:
            void fileOrFolderPopUp() {
                if (ImGui::BeginPopup(StringUtils::string(POPUP_CHOOSE_TYPE_NAME).c_str())) {
                    if (ImGui::MenuItem(StringUtils::string(FILE_POPUP_OPTION_NAME).c_str())) {
                        auto files = Selector::selectMultipleFiles();
                        if (files) {
                            if (files->size() == 1) {
                                ApplicationRegistry::push(ApplicationRegistry::Key::FILE_TO_ENCRYPT, (*files)[0]);
                            } else {
                                ApplicationRegistry::push(ApplicationRegistry::Key::FILES_TO_ENCRYPT, *files);
                            }
                        }
                        ImGui::CloseCurrentPopup();
                    }
            
                    if (ImGui::MenuItem(StringUtils::string(FOLDER_POPUP_OPTION_NAME).c_str())) {
                        auto folder = Selector::selectFolder();
                        if (folder) {
                            ApplicationRegistry::push(ApplicationRegistry::Key::FOLDER_TO_ENCRYPT, (*folder)[0]);
                        }
                        ImGui::CloseCurrentPopup();
                    }
            
                    ImGui::EndPopup();
                }

                if (
                    ApplicationRegistry::containsAny(
                        ApplicationRegistry::Key::FOLDER_TO_ENCRYPT,
                        ApplicationRegistry::Key::FILE_TO_ENCRYPT,
                        ApplicationRegistry::Key::FILES_TO_ENCRYPT
                    )
                ) {
                    FunctionalGuiWindow::getInstance().action = FunctionalWindowAction::ENCRYPT;
                }
            }

            void decryptButton() {
                auto file = Selector::selectFeFile();
                if (file) {
                    ApplicationRegistry::push(ApplicationRegistry::Key::FILE_TO_DECRYPT, *file);
                    FunctionalGuiWindow::getInstance().action = FunctionalWindowAction::DECTYPT;
                }
            }

            void temporaryOpenButton() {
                auto file = Selector::selectFeFile();
                if (file) {
                    ApplicationRegistry::push(ApplicationRegistry::Key::FILE_TO_DECRYPT, *file);
                    FunctionalGuiWindow::getInstance().action = FunctionalWindowAction::TEMPORARY_OPEN;
                }
            }
    };
}