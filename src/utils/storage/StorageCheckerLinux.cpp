#include "StorageChecker.hpp"

#if defined(__LINUX__)
#include <fstream>
#include <string>
#include <filesystem>

namespace fe {
    bool isRunningFromRemovableMedia() {
        using namespace std::filesystem;

        std::error_code ec;
        path exePath = read_symlink("/proc/self/exe", ec);
        if (ec || !exists(exePath)) return false;

        std::ifstream mounts("/proc/mounts");
        std::string line;
        while (std::getline(mounts, line)) {
            if ((line.find("vfat") != std::string::npos || line.find("exfat") != std::string::npos) &&
                line.find(exePath.root_path().string()) != std::string::npos)
            {
                return true;
            }
        }

        return false;
    }
}
#endif