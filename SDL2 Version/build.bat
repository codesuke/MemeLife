@echo off
setlocal enabledelayedexpansion

REM MemeLife SDL2 Project - Cross-Platform Build Script
REM This script automatically finds Visual Studio and build tools

cd /d "%~dp0"

REM Find Visual Studio installation
set "VCVARS_PATH="
for %%D in (2022 2019 2017) do (
    if exist "C:\Program Files\Microsoft Visual Studio\%%D\*\VC\Auxiliary\Build\vcvarsall.bat" (
        for /d %%P in ("C:\Program Files\Microsoft Visual Studio\%%D\*") do (
            if exist "%%P\VC\Auxiliary\Build\vcvarsall.bat" (
                set "VCVARS_PATH=%%P\VC\Auxiliary\Build\vcvarsall.bat"
                goto :found_vs
            )
        )
    )
)

for %%D in (2022 2019 2017) do (
    if exist "C:\Program Files (x86)\Microsoft Visual Studio\%%D\*\VC\Auxiliary\Build\vcvarsall.bat" (
        for /d %%P in ("C:\Program Files (x86)\Microsoft Visual Studio\%%D\*") do (
            if exist "%%P\VC\Auxiliary\Build\vcvarsall.bat" (
                set "VCVARS_PATH=%%P\VC\Auxiliary\Build\vcvarsall.bat"
                goto :found_vs
            )
        )
    )
)

:found_vs
if "!VCVARS_PATH!"=="" (
    echo Error: Could not find Visual Studio installation
    echo Please install Visual Studio Build Tools or Visual Studio Community
    exit /b 1
)

echo [Build] Found Visual Studio at: !VCVARS_PATH!
call "!VCVARS_PATH!" x64

REM Find CMake and Ninja in vcpkg downloads
setlocal enabledelayedexpansion
for /d %%D in ("vcpkg\downloads\tools\cmake-*") do set "CMAKE_DIR=%%D"
for /d %%D in ("vcpkg\downloads\tools\ninja\*") do set "NINJA_DIR=%%D"

if not exist "!CMAKE_DIR!" (
    echo Error: CMake not found in vcpkg\downloads\tools\cmake-*
    echo Please run setup.bat or setup.ps1 first
    exit /b 1
)

if not exist "!NINJA_DIR!" (
    echo Error: Ninja not found in vcpkg\downloads\tools\ninja\*
    echo Please run setup.bat or setup.ps1 first
    exit /b 1
)

echo [Build] Found CMake at: !CMAKE_DIR!
echo [Build] Found Ninja at: !NINJA_DIR!

REM Create build directory if needed
if not exist "build" mkdir build
cd /d "build"

REM Configure with CMake
echo [Build] Configuring project...
"!CMAKE_DIR!\bin\cmake.exe" .. -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM="!NINJA_DIR!\ninja.exe"
if errorlevel 1 (
    echo Error: CMake configuration failed
    exit /b 1
)

REM Build with Ninja
echo [Build] Building project...
"!NINJA_DIR!\ninja.exe"
if errorlevel 1 (
    echo Error: Build failed
    exit /b 1
)

echo [Build] Build complete! Executable: build\MemeLifeSDL2.exe
endlocal