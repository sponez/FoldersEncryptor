#include "StorageChecker.hpp"

#if defined(_WIN32)
    #include "StorageCheckerWindows.cpp"
#elif defined(__APPLE__)
    #include "StorageCheckerMac.cpp"
#else
    #include "StorageCheckerLinux.cpp"
#endif