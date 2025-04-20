cmake_minimum_required(VERSION 3.14)
project(FoldersEncryptor)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXE_LINKER_FLAGS "-mwindows")

# Include
include_directories(
    vendor/imgui
    vendor/imgui/backends
    vendor/SDL2/include
    vendor/nfd/src/include
)

# ImGui
set(IMGUI_SOURCES
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_demo.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/backends/imgui_impl_sdl2.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
)

# App
set(APP_SOURCES
    src/main.cpp
    src/application/Application.hpp

    src/gui/GuiGlobalController.hpp
    src/gui/imgui/ImguiController.cpp
    src/gui/sdl/SdlController.cpp
    
    src/gui/window/GuiUtils.hpp
    src/gui/window/GuiWindowId.hpp
    src/gui/window/abstract/GuiWindow.hpp
    src/gui/window/abstract/GuiWindowController.hpp
    src/gui/window/main/MainGuiWindow.hpp
    src/gui/window/main/MainGuiWindowController.hpp
    src/gui/window/main/MainWindowAction.hpp
    
    src/selector/Selector.cpp

    src/core/enctyption/encryptor/Encryptor.tpp
    src/core/enctyption/decryptor/Decryptor.cpp
    src/core/enctyption/utils/EncryptionUtils.cpp
    src/core/controller/encryptor/EncryptionController.cpp
    src/core/controller/decryptor/DecryptionController.cpp
    src/core/controller/utils/ControllerUtils.cpp

    src/core/model/chunk/Chunk.cpp
    src/core/serialization/serializer/ChunkSerializer.cpp
    src/core/serialization/deserializer/ChunkDeserializer.cpp
    src/core/io/writer/EncryptingWriter.cpp
    src/core/io/reader/DecryptingReader.cpp
    src/core/processor/Processor.cpp
    src/core/processor/explorer/ExplorerTool.cpp

    src/core/multithreading/BlockingStrictIndexedQueue.hpp

    src/utils/files/FilesUtils.cpp
    src/utils/properties/Properties.hpp
    src/utils/properties/PropertiesLoader.hpp
    src/utils/properties/PropertiesFileBuilder.hpp
    src/utils/string/StringUtils.hpp
)

# SDL2
set(SDL2_STATIC_DIR ${CMAKE_SOURCE_DIR}/vendor/SDL2/build-mingw-static)
set(SDL2MAIN_LIB ${SDL2_STATIC_DIR}/libSDL2main.a)
set(SDL2_LIB ${SDL2_STATIC_DIR}/libSDL2.a)

# NFD
set(NFD_SOURCES
    vendor/nfd/src/nfd_common.c
    vendor/nfd/src/nfd_win.cpp
)

# Sodium
include_directories(${CMAKE_SOURCE_DIR}/vendor/sodium/include)
link_directories(${CMAKE_SOURCE_DIR}/vendor/sodium/lib)

# RapidJson
include_directories(${CMAKE_SOURCE_DIR}/vendor/rapidjson/include)

# Threads
find_package(Threads REQUIRED)

# Executable
add_executable(FoldersEncryptor
    ${APP_SOURCES}
    ${IMGUI_SOURCES}
)

# Target
target_sources(FoldersEncryptor
    PRIVATE
        ${NFD_SOURCES}
)
target_link_libraries(FoldersEncryptor
    ${SDL2_LIB}
    setupapi
    cfgmgr32
    opengl32
    imm32
    winmm
    version
    sodium
    Threads::Threads
)
