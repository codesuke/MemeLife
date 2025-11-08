@echo off
REM MemeLife SDL2 Project - Setup Script
REM This script automates the entire setup process for new developers

echo.
echo ============================================================
echo   MemeLife SDL2 Project - Setup Wizard
echo ============================================================
echo.

REM Check if vcpkg exists
if not exist "vcpkg" (
    echo [1/4] Cloning vcpkg...
    git clone https://github.com/Microsoft/vcpkg.git
    cd vcpkg
    call .\bootstrap-vcpkg.bat
    cd ..
) else (
    echo [1/4] vcpkg already exists, skipping clone
)

REM Check if packages are installed
if not exist "vcpkg\installed\x64-windows" (
    echo [2/4] Installing SDL2 dependencies...
    call vcpkg\vcpkg.exe install SDL2:x64-windows SDL2-image:x64-windows SDL2-mixer:x64-windows
) else (
    echo [2/4] SDL2 packages already installed
)

REM Check for SDL2_ttf
if not exist "vcpkg\installed\x64-windows\include\SDL2\SDL_ttf.h" (
    echo [3/4] Setting up SDL2_ttf manually...
    if not exist "vcpkg\downloads" mkdir vcpkg\downloads
    cd vcpkg\downloads
    
    REM Download SDL2_ttf if not present
    if not exist "SDL2_ttf-2.24.0.zip" (
        powershell -Command "Invoke-WebRequest -Uri 'https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.24.0/SDL2_ttf-devel-2.24.0-VC.zip' -OutFile 'SDL2_ttf-2.24.0.zip'"
    )
    
    REM Extract if not extracted
    if not exist "..\installed\x64-windows\include\SDL2\SDL_ttf.h" (
        powershell -Command "Expand-Archive -Path SDL2_ttf-2.24.0.zip -DestinationPath ."
        xcopy SDL2_ttf-2.24.0\include\*.h ..\installed\x64-windows\include\SDL2\ /Y
        xcopy SDL2_ttf-2.24.0\lib\x64\*.lib ..\installed\x64-windows\lib\ /Y
        xcopy SDL2_ttf-2.24.0\lib\x64\*.dll ..\installed\x64-windows\bin\ /Y
    )
    
    cd ..\..
) else (
    echo [3/4] SDL2_ttf already installed
)

REM Create build directory
if not exist "build" (
    echo [4/4] Creating build directory and running CMake...
    mkdir build
) else (
    echo [4/4] Build directory exists, cleaning it...
    cd build
    del /s /q * >nul 2>&1
    cd ..
)

REM Configure and build
echo.
echo Running CMake configuration...
cd build
"..\vcpkg\downloads\tools\cmake-3.30.1-windows\cmake-3.30.1-windows-i386\bin\cmake.exe" .. -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM="..\vcpkg\downloads\tools\ninja\1.13.1-windows\ninja.exe"
cd ..

echo.
echo ============================================================
echo   Setup Complete!
echo ============================================================
echo.
echo Next steps:
echo   1. Run 'build.bat' to compile the project
echo   2. Run 'build\MemeLifeSDL2.exe' to start the game
echo.
pause
