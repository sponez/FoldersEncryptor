#pragma once

#include <Windows.h>
#include <shldisp.h>
#include <comdef.h>
#include <shlobj.h>
#include <comutil.h>

#include <filesystem>
#include <string>
#include <algorithm>
#include <iostream>
#include <thread>

namespace fe {
    class ExplorerTool {
        public:
            static void openPathInExplorer(const std::filesystem::path& folderPath);
            static bool isFolderOpenInExplorer(const std::filesystem::path& targetPath);

        private:
            static void initFolderWindowIndex(IShellWindows* shellWindows, IDispatch* disp, long& lIndex, VARIANT& index);
            static void release(VARIANT* index = nullptr, IDispatch* disp = nullptr, IWebBrowserApp* browser = nullptr);
    };
}