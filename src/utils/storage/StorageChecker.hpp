#pragma once

#include <optional>
#include <string>

#include "../string/StringUtils.hpp"

namespace fe {
    bool isRunningFromRemovableMedia();
    std::optional<std::u8string> getStorageDeviceId();
}