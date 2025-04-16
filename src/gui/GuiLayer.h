#pragma once

#include "../application/ApplicationConstants.h"
#include "WindowsEnum.h"
#include <array>
#include <vector>
#include <filesystem>
#include <imgui.h>
#include <thread>
#include <atomic>

class GuiLayer {
public:
    void imGuiWindow();

private:
    void mainWindow();
    
    void encryptButton();
    void dectyptButton();
    void openButton();
    void preferencesButton();

    bool chooseTypePopup();

    void encryptDataWindow();
    void decryptDataWindow();
    void temporaryDecryptDataWindow();
    void preferencesWindow();

    void findTotalBytes();
    void progressBar();

private:
    ImVec2 displaySize;
    float buttonWidth;
    float buttonHeight;

    Window currentWindow = Window::MAIN;
    std::vector<std::filesystem::path> selectedPaths;
    std::array<char, 256> outputName = {};
    std::array<char, 256> password = {};
    bool isFolder = false;

    int threadCount = 1;
    int bufferSizePercent = 10;

    std::atomic<size_t> bytesProcessed = 0;
    std::size_t totalBytes = 0;
    bool inProgress = false;
};