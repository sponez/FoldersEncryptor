#include "ExplorerTool.h"

namespace fe {
    void ExplorerTool::openPathInExplorer(const std::filesystem::path& folderPath) {
        std::string tempFolderName = "temp";
        std::filesystem::path tempFolder = folderPath / tempFolderName;
        std::filesystem::create_directories(tempFolder);

        while (!std::filesystem::exists(tempFolder)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        HRESULT init = CoInitialize(nullptr);
        if (FAILED(init)) {
            throw std::runtime_error("Failed to open folder");
        }

        PIDLIST_ABSOLUTE pidl = nullptr;
        HRESULT hr = SHParseDisplayName(tempFolder.c_str(), nullptr, &pidl, 0, nullptr);

        if (SUCCEEDED(hr)) {
            hr = SHOpenFolderAndSelectItems(pidl, 0, nullptr, 0);
            CoTaskMemFree(pidl);
        }

        CoUninitialize();

        std::filesystem::remove_all(tempFolder);
    }

    bool ExplorerTool::isFolderOpenInExplorer(const std::filesystem::path& targetPath) {
        CoInitialize(nullptr);
        IShellWindows* shellWindows = nullptr;
        HRESULT hr = CoCreateInstance(CLSID_ShellWindows, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&shellWindows));

        if (SUCCEEDED(hr)) {
            long count = 0;
            shellWindows->get_Count(&count);

            for (long i = 0; i < count; ++i) {
                VARIANT index;
                IDispatch* disp = nullptr;
                IWebBrowserApp* browser = nullptr;

                initFolderWindowIndex(shellWindows, disp, i, index);
                if (shellWindows->Item(index, &disp) != S_OK || !disp) {
                    release(&index, disp);
                    continue;
                }

                if (disp->QueryInterface(IID_PPV_ARGS(&browser)) != S_OK || !browser) {
                    release(&index, disp, browser);
                    continue;
                }

                BSTR bstr;
                if (browser->get_LocationURL(&bstr) != S_OK) {
                    release(&index, disp, browser);
                    continue;
                }

                _bstr_t url(bstr, false);
                std::wstring wurl(bstr, SysStringLen(bstr));
                SysFreeString(bstr);
                if (wurl.find(L"file:///", 0) != 0) {
                    release(&index, disp, browser);
                    continue;
                }

                std::wstring path = wurl.substr(8);
                std::replace(path.begin(), path.end(), '/', '\\');

                std::filesystem::path current = path;
                if (std::filesystem::equivalent(current, targetPath)) {
                    release(&index, disp, browser);
                    shellWindows->Release();
                    return true;
                }
            }

            shellWindows->Release();
        }

        return false;
    }

    void ExplorerTool::initFolderWindowIndex(IShellWindows* shellWindows, IDispatch* disp, long& lIndex, VARIANT& index) {
        VariantInit(&index);
        index.vt = VT_I4;
        index.lVal = lIndex;
        shellWindows->Item(index, &disp);
    }

    void ExplorerTool::release(VARIANT* index, IDispatch* disp, IWebBrowserApp* browser) {
        if (index) { VariantClear(index); }
        if (disp) { disp->Release(); }
        if (browser) { browser -> Release(); }
    }
}