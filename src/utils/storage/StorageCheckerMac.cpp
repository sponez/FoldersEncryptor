#include "StorageChecker.hpp"

#if defined(__APPLE__)
#include <sys/mount.h>
#include <unistd.h>
#include <string>
#include <limits.h>

namespace fe {
    bool isRunningFromRemovableMedia() {
        char exePath[PATH_MAX];
        uint32_t size = sizeof(exePath);
        if (_NSGetExecutablePath(exePath, &size) != 0) return false;

        struct statfs buf;
        if (statfs(exePath, &buf) != 0) return false;

        std::string fsType = buf.f_fstypename;
        return fsType == "msdos" || fsType == "exfat";
    }
}
#endif