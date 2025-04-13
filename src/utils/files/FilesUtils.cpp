#include "FilesUtils.h"

std::filesystem::path FilesUtils::determineOutputPath(
    const std::vector<std::filesystem::path>& inputPaths,
    const std::string& outputFilename
) {
    if (inputPaths.empty()) {
        return "";
    }

    return inputPaths[0].parent_path() / outputFilename;
}

std::map<std::filesystem::path, std::filesystem::path> FilesUtils::createAbsoluteToRelativePathMap(const std::vector<std::filesystem::path>& selectedPaths) {
    std::map<std::filesystem::path, std::filesystem::path> result;

    for (const auto& path : selectedPaths) {
        if (std::filesystem::is_directory(path)) {
            for (auto& entry : std::filesystem::recursive_directory_iterator(path)) {
                if (entry.is_regular_file()) {
                    std::filesystem::path relative = std::filesystem::relative(entry.path(), path);
                    result[entry.path()] = relative;
                }
            }
        } else if (std::filesystem::is_regular_file(path)) {
            result[path] = path.filename();
        }
    }

    return result;
}

uint64_t FilesUtils::findFileSize(const std::filesystem::path &path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        return 0;
    }

    in.seekg(0, std::ios::end);
    uint64_t fileSize = in.tellg();
    in.close();

    return fileSize;
}
