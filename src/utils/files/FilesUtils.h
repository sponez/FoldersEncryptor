#pragma once

#include <filesystem>
#include <vector>
#include <windows.h>

class FilesUtils {
public:
    static std::vector<std::filesystem::path> unpack(std::filesystem::path& folder);
    static std::size_t getAvailableRAMBytes();

private:
    FilesUtils() {}
};