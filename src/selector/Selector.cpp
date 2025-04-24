#include "Selector.h"

namespace fe {
    std::optional<std::vector<std::filesystem::path>> Selector::selectFolder() {
        nfdchar_t* outPath = nullptr;
        nfdresult_t result = NFD_PickFolder(nullptr, &outPath);

        if (result == NFD_OKAY) {
            std::vector<std::filesystem::path> paths = { std::filesystem::path(outPath) };
            free(outPath);
            return paths;
        } else if (result == NFD_CANCEL) {
            return std::nullopt;
        } else {
            return std::nullopt;
        }
    }

    std::optional<std::vector<std::filesystem::path>> Selector::selectMultipleFiles() {
        nfdpathset_t outPaths;
        nfdresult_t result = NFD_OpenDialogMultiple(nullptr, nullptr, &outPaths);
        
        if (result == NFD_OKAY) {
            std::vector<std::filesystem::path> paths;
            for (size_t i = 0; i < NFD_PathSet_GetCount(&outPaths); ++i) {
                paths.emplace_back(NFD_PathSet_GetPath(&outPaths, i));
            }
            NFD_PathSet_Free(&outPaths);
            return paths;
        } else if (result == NFD_CANCEL) {
            return std::nullopt;
        } else {
            return std::nullopt;
        }
    }

    std::optional<std::filesystem::path> Selector::selectFeFile() {
        nfdchar_t* outPath = nullptr;
        const nfdresult_t result = NFD_OpenDialog(StringUtils::string(FILENAME_EXTENSION).c_str(), nullptr, &outPath);

        if (result == NFD_OKAY) {
            std::filesystem::path path(outPath);
            free(outPath);
            return path;
        }

        return std::nullopt;
    }
}
