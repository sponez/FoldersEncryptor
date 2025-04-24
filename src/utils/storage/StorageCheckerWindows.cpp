#include "StorageChecker.hpp"

#if defined(_WIN32)
#include <windows.h>
#include <winioctl.h>
#include <string>
#include <filesystem>

namespace fe {
    bool isRunningFromRemovableMedia() {
        char path[MAX_PATH];
        if (!GetModuleFileNameA(NULL, path, MAX_PATH)) {
            return false;   
        }

        char rootPath[4] = { path[0], ':', '\\', '\0' };
        UINT driveType = GetDriveTypeA(rootPath);

        return driveType == DRIVE_REMOVABLE;
    }

    std::optional<std::u8string> getStorageDeviceId() {
        char path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);

        char driveLetter = path[0]; // 'C', 'D', ...
        std::string devicePath = std::string(R"(\\.\)") + driveLetter + ":";

        HANDLE hDevice = CreateFileA(
            devicePath.c_str(),
            0,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            nullptr,
            OPEN_EXISTING,
            0,
            nullptr
        );

        if (hDevice == INVALID_HANDLE_VALUE) {
            return std::nullopt;
        }

        STORAGE_PROPERTY_QUERY query{};
        query.PropertyId = StorageDeviceProperty;
        query.QueryType = PropertyStandardQuery;

        BYTE buffer[1024];
        DWORD bytesReturned = 0;

        if (
            !DeviceIoControl(
                hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
                &query, sizeof(query),
                buffer, sizeof(buffer),
                &bytesReturned, nullptr
            )
        ) {
            CloseHandle(hDevice);
            return std::nullopt;
        }

        CloseHandle(hDevice);

        auto desc = reinterpret_cast<STORAGE_DEVICE_DESCRIPTOR*>(buffer);
        if (desc->SerialNumberOffset == 0) {
            return std::nullopt;
        }

        return StringUtils::u8string(std::string(reinterpret_cast<char*>(buffer + desc->SerialNumberOffset)));
    }
}
#endif