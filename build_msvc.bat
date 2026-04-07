@echo off
setlocal

if not exist vcpkg (
    git clone https://github.com/microsoft/vcpkg.git
    if errorlevel 1 (
        echo Failed Git Clone
        exit /b 1
    )
)

if not exist vcpkg\vcpkg.exe (
    pushd vcpkg
    call bootstrap-vcpkg.bat -disableMetrics
    if errorlevel 1 (
        echo Failed Bootstrap
        exit /b 1
    )
    popd
)

vcpkg\vcpkg install vcpkg-cmake vcpkg-cmake-config
vcpkg\vcpkg install sdl2

if errorlevel 1 (
    echo Failed SDL2 Install
    exit /b 1
)

if exist build (
    rmdir /s /q build
)

mkdir build

set "tool=%~dp0vcpkg\scripts\buildsystems\vcpkg.cmake"

cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="%tool%" -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

endlocal

