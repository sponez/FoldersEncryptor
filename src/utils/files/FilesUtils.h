#pragma once

#include <filesystem>
#include <fstream>
#include <array>
#include <vector>
#include <map>
#include <optional>

class FilesUtils {
public:
    static std::filesystem::path determineOutputPath(const std::vector<std::filesystem::path>& inputPaths, const std::string& outputFilename);
    static uint64_t findFileSize(const std::filesystem::path& path);
    static std::map<std::filesystem::path, std::filesystem::path> createAbsoluteToRelativePathMap(const std::vector<std::filesystem::path>& selectedPaths);

private:
    FilesUtils() {}
};