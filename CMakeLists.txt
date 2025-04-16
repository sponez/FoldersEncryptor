cmake_minimum_required(VERSION 3.14)
project(FoldersEncryptor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_EXE_LINKER_FLAGS "-mwindows")

# ──────── INCLUDE ────────
include_directories(
    vendor/imgui
    vendor/imgui/backends
    vendor/SDL2/include
    vendor/nfd/src/include
)

# ──────── SOURCES ────────
set(IMGUI_SOURCES
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_demo.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/backends/imgui_impl_sdl2.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
)

set(APP_SOURCES
    src/main.cpp
    src/application/Application.cpp
    src/gui/GuiLayer.cpp
    
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
)

# ──────── SDL2 STATIC ────────
set(SDL2_STATIC_DIR ${CMAKE_SOURCE_DIR}/vendor/SDL2/build-mingw-static)
set(SDL2MAIN_LIB ${SDL2_STATIC_DIR}/libSDL2main.a)
set(SDL2_LIB ${SDL2_STATIC_DIR}/libSDL2.a)

# ──────────────── NFD ────────────────
set(NFD_SOURCES
    vendor/nfd/src/nfd_common.c
    vendor/nfd/src/nfd_win.cpp
)

# ──────────────── SODIUM ────────────────
set(VCPKG_INSTALLED_DIR "P:/DevelopmentTools/CppLibs/vcpkg/installed/x64-mingw-static")
include_directories(${VCPKG_INSTALLED_DIR}/include)
link_directories(${VCPKG_INSTALLED_DIR}/lib)

# ──────────────── THREADS ────────────────
find_package(Threads REQUIRED)

# ──────── EXECUTABLE ────────
add_executable(FoldersEncryptor
    ${APP_SOURCES}
    ${IMGUI_SOURCES}
)

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
