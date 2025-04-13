#pragma once

#include <optional>
#include <string>
#include <vector>
#include <filesystem>

class Selector {
public:
    static std::optional<std::vector<std::filesystem::path>> selectFolder();
    static std::optional<std::vector<std::filesystem::path>> selectMultipleFiles();
    static std::optional<std::filesystem::path> selectFeFile();

private:
    Selector() {}
};