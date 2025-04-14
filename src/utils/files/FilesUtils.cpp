#include "FilesUtils.h"

std::vector<std::filesystem::path> FilesUtils::unpack(std::filesystem::path& folder) {
    std::vector<std::filesystem::path> unpacked;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(folder)) {
        if (std::filesystem::is_regular_file(entry)) {
            unpacked.push_back(std::filesystem::absolute(entry.path()));
        }
    }

    return unpacked;
}
