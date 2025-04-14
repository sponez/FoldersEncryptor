#pragma once

#include "../application/ApplicationConstants.h"
#include "WindowsEnum.h"
#include <array>
#include <vector>
#include <filesystem>
#include <imgui.h>

class GuiLayer {
public:
    void imGuiWindow();

private:
    void mainWindow();
    void encryptButton();
    void dectyptButton();
    void openButton();
    bool chooseTypePopup();

    void encryptDataWindow();
    void decryptDataWindow();
    void temporaryDecryptDataWindow();

private:
    ImVec2 displaySize;
    float buttonWidth;
    float buttonHeight;

    Window currentWindow = Window::MAIN;
    std::vector<std::filesystem::path> selectedPaths;
    std::array<char, 256> outputName = {};
    std::array<char, 256> password = {};
};