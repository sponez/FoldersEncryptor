#pragma once

#include <filesystem>
#include <fstream>
#include <array>
#include <vector>
#include <map>
#include <optional>

class FilesUtils {
public:
    static std::vector<std::filesystem::path> unpack(std::filesystem::path& folder);

private:
    FilesUtils() {}
};