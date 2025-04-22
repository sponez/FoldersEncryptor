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
            inline static const std::string ENRYPT_BUTTON_NAME = "Enctypt";

            inline static const std::string POPUP_CHOOSE_TYPE_NAME = "Choose type";
            inline static const std::string FILE_POPUP_OPTION_NAME = "File(s)";
            inline static const std::string FOLDER_POPUP_OPTION_NAME = "Folder";

            inline static const std::string DERYPT_BUTTON_NAME = "Decrypt";
            inline static const std::string TEMPORARY_OPEN_BUTTON_NAME = "Temporary open";
            inline static const std::string PROPERTIES_BUTTON_NAME = "Properties";

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
                    []() { FunctionalGuiWindow::getInstance().action = FunctionalWindowAction::BACK; }
                );

                GuiUtils::itemGroup(
                    {
                        std::make_shared<GuiUtils::Button>(
                            ENRYPT_BUTTON_NAME,
                            [&]() {
                                ImGui::OpenPopup(POPUP_CHOOSE_TYPE_NAME.c_str());
                            }
                        ),
                        std::make_shared<GuiUtils::Button>(
                            DERYPT_BUTTON_NAME,
                            [&]() {
                                decryptButton();
                            }
                        ),
                        std::make_shared<GuiUtils::Button>(
                            TEMPORARY_OPEN_BUTTON_NAME,
                            [&]() {
                                temporaryOpenButton();
                            }
                        ),
                        std::make_shared<GuiUtils::Button>(
                            PROPERTIES_BUTTON_NAME,
                            []() {
                                FunctionalGuiWindow::getInstance().action = FunctionalWindowAction::PROPERTIES;
                            }
                        )
                    }
                );

                fileOrFolderPopUp();
            }

        private:
            void fileOrFolderPopUp() {
                if (ImGui::BeginPopup(POPUP_CHOOSE_TYPE_NAME.c_str())) {
                    if (ImGui::MenuItem(FILE_POPUP_OPTION_NAME.c_str())) {
                        auto files = Selector::selectMultipleFiles();
                        if (files) {
                            ApplicationRegistry::filesToEncrypt = *files;
                        }
                        ImGui::CloseCurrentPopup();
                    }
            
                    if (ImGui::MenuItem(FOLDER_POPUP_OPTION_NAME.c_str())) {
                        auto folder = Selector::selectFolder();
                        if (folder) {
                            ApplicationRegistry::filesToEncrypt = *folder;
                        }
                        ImGui::CloseCurrentPopup();
                    }
            
                    ImGui::EndPopup();
                }

                if (!ApplicationRegistry::filesToEncrypt.empty()) {
                    FunctionalGuiWindow::getInstance().action = FunctionalWindowAction::ENCRYPT;
                }
            }

            void decryptButton() {
                ApplicationRegistry::fileToDecrypt = Selector::selectFeFile();
                if (ApplicationRegistry::fileToDecrypt) {
                    FunctionalGuiWindow::getInstance().action = FunctionalWindowAction::DECTYPT;
                }
            }

            void temporaryOpenButton() {
                ApplicationRegistry::fileToDecrypt = Selector::selectFeFile();
                if (ApplicationRegistry::fileToDecrypt) {
                    FunctionalGuiWindow::getInstance().action = FunctionalWindowAction::TEMPORARY_OPEN;
                }
            }
    };
}