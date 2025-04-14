#include "GuiLayer.h"
#include "GuiConstants.h"
#include "../selector/Selector.h"
#include "../core/processor/Processor.h"
#include <iostream>
#include <optional>
#include <stdlib.h>

void GuiLayer::imGuiWindow() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin(
        BEGIN, nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoTitleBar
    );
    ImGui::SetCursorPos(ImVec2(0, 0));

    mainWindow();

    ImGui::End();
}

void GuiLayer::mainWindow() {
    displaySize = ImGui::GetIO().DisplaySize;
    buttonWidth = displaySize.x * 0.2f;
    buttonHeight = displaySize.y * 0.05f;

    ImGui::Text(CHOOSE_ACTION);

    switch (currentWindow)
    {
    case MAIN:
        encryptButton();
        dectyptButton();
        openButton();
        break;
    
    case ENCRYPT_DATA:
        encryptDataWindow();
        break;

    case DECRYPT_DATA:
        decryptDataWindow();
        break;

    case TEMPORARY_DECRYPT_DATA:
        temporaryDecryptDataWindow();
        break;

    default:
        break;
    }

    if (!selectedPaths.empty()) {
        ImGui::Separator();
        ImGui::Text(SELECTED);
        for (const auto& path : selectedPaths) {
            ImGui::Text("%s", path.u8string().c_str());
        }
    }
}

void GuiLayer::encryptButton()
{
    if (ImGui::Button(ENCRYPT_BUTTON, ImVec2(buttonWidth, buttonHeight))) {
        ImGui::OpenPopup(CHOOSE_TYPE);
    }

    if (chooseTypePopup() && selectedPaths.size() > 0) {
        currentWindow = Window::ENCRYPT_DATA;
    }
}

void GuiLayer::dectyptButton() {
    if (ImGui::Button(DECRYPT_BUTTON, ImVec2(buttonWidth, buttonHeight))) {
        auto file = Selector::selectFeFile();
        if (file) {
            selectedPaths.push_back(*file);
            currentWindow = Window::DECRYPT_DATA;
        }
    }
}

void GuiLayer::openButton() {
    if (ImGui::Button(OPEN_BUTTON, ImVec2(buttonWidth, buttonHeight))) {
        auto file = Selector::selectFeFile();
        if (file) {
            selectedPaths.push_back(*file);
            currentWindow = Window::TEMPORARY_DECRYPT_DATA;
        }
    }
}

bool GuiLayer::chooseTypePopup() {
    bool result = false;

    if (ImGui::BeginPopup(CHOOSE_TYPE)) {
        if (ImGui::MenuItem(FOLDER)) {
            auto folder = Selector::selectFolder();
            if (folder) {
                selectedPaths = *folder;
            }
            ImGui::CloseCurrentPopup();
            result = !(folder == std::nullopt);
        }

        if (ImGui::MenuItem(FILES)) {
            auto files = Selector::selectMultipleFiles();
            if (files) {
                selectedPaths = *files;
            }
            ImGui::CloseCurrentPopup();
            result = !(files == std::nullopt);
        }

        ImGui::EndPopup();
    }

    return result;
}

void GuiLayer::encryptDataWindow() {
    std::string outputNameString;

    if (selectedPaths.size() == 1) {
        ImGui::InputText(PASSWORD, password.data(), password.size());

        outputNameString = selectedPaths[0]
            .generic_u8string()
            .append(".")
            .append(FILENAME_EXTENSION);
    } else {
        ImGui::InputText(OUTPUT_FILE_NAME, outputName.data(), outputName.size());
        ImGui::InputText(PASSWORD, password.data(), password.size(), ImGuiInputTextFlags_Password);

        outputNameString = outputName.data();
    }

    if (ImGui::Button(OK, ImVec2(buttonWidth, buttonHeight))) {
        std::size_t bufferSize = 4096;
        std::filesystem::path root = selectedPaths[0].parent_path();
        fe::Processor::processEncryptOption(
            outputNameString,
            selectedPaths[0].parent_path(),
            selectedPaths,
            password,
            bufferSize
        );
        std::fill(outputName.data(), outputName.data() + outputName.size(), '\0');
        currentWindow = Window::MAIN;
        selectedPaths.clear();
    }
}

void GuiLayer::decryptDataWindow() {
    if (!selectedPaths.empty()) {
        ImGui::InputText(PASSWORD, password.data(), password.size());
    }

    if (ImGui::Button(OK, ImVec2(buttonWidth, buttonHeight))) {
        fe::Processor::processDecryptOption(
            selectedPaths[0].parent_path(),
            selectedPaths[0],
            password
        );

        currentWindow = Window::MAIN;
        selectedPaths.clear();
    }
}

void GuiLayer::temporaryDecryptDataWindow() {
    if (!selectedPaths.empty()) {
        ImGui::InputText(PASSWORD, password.data(), password.size());
    }

    if (ImGui::Button(OK, ImVec2(buttonWidth, buttonHeight))) {
        fe::Processor::processTemporaryDecryptOption(
            selectedPaths[0],
            password
        );

        currentWindow = Window::MAIN;
        selectedPaths.clear();
    }
}
