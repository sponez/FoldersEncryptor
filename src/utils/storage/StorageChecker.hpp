#pragma once

#include <optional>
#include <string>

namespace fe {
    bool isRunningFromRemovableMedia();
    std::optional<std::string> getStorageDeviceId();
}