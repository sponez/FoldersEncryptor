#include "GuiLayer.h"
#include "GuiConstants.h"
#include "../selector/Selector.h"
#include "../core/processor/Processor.h"
#include "../utils/files/FilesUtils.h"
#include <iostream>
#include <optional>
#include <thread>
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
        preferencesButton();
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

    case PREFERENSES:
        preferencesWindow();
        break;

    case PROGRESS_BAR:
        progressBar();
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

void GuiLayer::preferencesButton() {
    if (ImGui::Button(PREFERENSES_BUTTON, ImVec2(buttonWidth, buttonHeight))) {
        currentWindow = Window::PREFERENSES;
    }
}

bool GuiLayer::chooseTypePopup() {
    bool result = false;

    if (ImGui::BeginPopup(CHOOSE_TYPE)) {
        if (ImGui::MenuItem(FOLDER)) {
            auto folder = Selector::selectFolder();
            if (folder) {
                selectedPaths = *folder;
                isFolder = true;
            }
            ImGui::CloseCurrentPopup();
            result = !(folder == std::nullopt);
        }

        if (ImGui::MenuItem(FILES)) {
            auto files = Selector::selectMultipleFiles();
            if (files) {
                selectedPaths = *files;
                isFolder = false;
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
    std::filesystem::path rootPath;
    bool isOk = false;

    if (selectedPaths.size() == 1) {
        ImGui::InputText(PASSWORD, password.data(), password.size(), ImGuiInputTextFlags_Password);

        outputNameString = selectedPaths[0].generic_u8string();
    } else {
        ImGui::InputText(OUTPUT_FILE_NAME, outputName.data(), outputName.size());
        ImGui::InputText(PASSWORD, password.data(), password.size(), ImGuiInputTextFlags_Password);

        outputNameString = outputName.data();
    }

    outputNameString += (std::string)"." + FILENAME_EXTENSION;
    rootPath = std::filesystem::path(selectedPaths[0].parent_path());

    if (ImGui::Button(OK, ImVec2(buttonWidth, buttonHeight))) {
        std::filesystem::path folderToDelete;
        if (isFolder) {
            folderToDelete = std::filesystem::path(selectedPaths[0]);
            selectedPaths = FilesUtils::unpack(selectedPaths[0]);
        }

        findTotalBytes();
        inProgress = true;
        std::size_t bufferSize = (FilesUtils::getAvailableRAMBytes() / 10 * bufferSizePercent / 100) / threadCount;
        std::thread worker(
            [outputNameString, rootPath, bufferSize, folderToDelete, this]() {
                fe::Processor::processEncryptOption(
                    outputNameString,
                    rootPath,
                    selectedPaths,
                    password,
                    bufferSize,
                    threadCount,
                    &bytesProcessed
                );

                if(isFolder) {
                    std::filesystem::remove_all(folderToDelete);
                }
                std::fill(outputName.data(), outputName.data() + outputName.size(), '\0');

                inProgress = false;
            }
        );
        worker.detach();
        currentWindow = Window::PROGRESS_BAR;
    }
}

void GuiLayer::decryptDataWindow() {
    if (!selectedPaths.empty()) {
        ImGui::InputText(PASSWORD, password.data(), password.size(), ImGuiInputTextFlags_Password);
    }

    if (ImGui::Button(OK, ImVec2(buttonWidth, buttonHeight))) {
        findTotalBytes();
        inProgress = true;
        std::thread worker(
            [&]() {
                fe::Processor::processDecryptOption(
                    selectedPaths[0].parent_path(),
                    selectedPaths[0],
                    password,
                    threadCount,
                    &bytesProcessed
                );

                inProgress = false;
            }
        );
        worker.detach();
        currentWindow = Window::PROGRESS_BAR;
    }
}

void GuiLayer::temporaryDecryptDataWindow() {
    if (!selectedPaths.empty()) {
        ImGui::InputText(PASSWORD, password.data(), password.size(), ImGuiInputTextFlags_Password);
    }

    if (ImGui::Button(OK, ImVec2(buttonWidth, buttonHeight))) {
        findTotalBytes();
        inProgress = true;
        std::thread worker(
            [&]() {
                fe::Processor::processTemporaryDecryptOption(
                    selectedPaths[0],
                    password,
                    threadCount,
                    &bytesProcessed
                );

                inProgress = false;
            }
        );
        worker.detach();

        currentWindow = Window::PROGRESS_BAR;
    }
}

void GuiLayer::preferencesWindow() {
    ImGui::SliderInt("Threads", &threadCount, 1, std::thread::hardware_concurrency());
    ImGui::SliderInt("BufferSize (%)", &bufferSizePercent, 1, 100);

    if (ImGui::Button(OK, ImVec2(buttonWidth, buttonHeight))) {
        currentWindow = Window::MAIN;
    }
}

void GuiLayer::findTotalBytes() {
    for (auto path: selectedPaths) {
        totalBytes += std::filesystem::file_size(path);
    }
}

void GuiLayer::progressBar() {
    if (inProgress) {
        float progress = static_cast<float>(bytesProcessed.load()) / totalBytes;
        progress = std::min(progress, 1.0f);
        ImGui::ProgressBar(progress, ImVec2(-1, 0));
        ImGui::Text("%.1f%%", progress * 100.0f);
    } else {
        currentWindow = Window::MAIN;
        bytesProcessed = 0;
        totalBytes = 0;
        selectedPaths.clear();
        isFolder = false;
    }
}
