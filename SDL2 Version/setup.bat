@echo off
setlocal enabledelayedexpansion
REM MemeLife SDL2 Project - Cross-Platform Setup Script
REM This script automates setup on any Windows system

echo.
echo ============================================================
echo   MemeLife SDL2 Project - Setup Wizard
echo ============================================================
echo.

cd /d "%~dp0"

REM Check for Git
where git >nul 2>&1
if errorlevel 1 (
    echo Error: Git is not installed or not in PATH
    echo Please install Git from https://git-scm.com/download/win
    exit /b 1
)

REM Step 1: Clone vcpkg
echo [1/5] Checking vcpkg...
if not exist "vcpkg" (
    echo Cloning vcpkg repository...
    git clone https://github.com/Microsoft/vcpkg.git
    if errorlevel 1 (
        echo Error: Failed to clone vcpkg
        exit /b 1
    )
    echo Bootstrapping vcpkg...
    call vcpkg\bootstrap-vcpkg.bat
    if errorlevel 1 (
        echo Error: Failed to bootstrap vcpkg
        exit /b 1
    )
) else (
    echo vcpkg already exists, skipping clone
)

REM Step 2: Install SDL2 packages
echo [2/5] Installing SDL2 packages...
if not exist "vcpkg\installed\x64-windows\lib\SDL2.lib" (
    echo Installing SDL2, SDL2-image, and SDL2-mixer...
    call vcpkg\vcpkg.exe install SDL2:x64-windows SDL2-image:x64-windows SDL2-mixer:x64-windows
    if errorlevel 1 (
        echo Error: Failed to install SDL2 packages
        exit /b 1
    )
) else (
    echo SDL2 packages already installed
)

REM Step 3: Setup SDL2_ttf
echo [3/5] Setting up SDL2_ttf...
if not exist "vcpkg\installed\x64-windows\include\SDL2\SDL_ttf.h" (
    if not exist "vcpkg\downloads" mkdir vcpkg\downloads
    
    cd vcpkg\downloads
    
    REM Download SDL2_ttf if not present
    if not exist "SDL2_ttf-2.24.0.zip" (
        echo Downloading SDL2_ttf from GitHub...
        powershell -NoProfile -Command "Invoke-WebRequest -Uri 'https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.24.0/SDL2_ttf-devel-2.24.0-VC.zip' -OutFile 'SDL2_ttf-2.24.0.zip'"
        if errorlevel 1 (
            echo Error: Failed to download SDL2_ttf
            cd ..\..
            exit /b 1
        )
    )
    
    REM Extract if not extracted
    if not exist "..\installed\x64-windows\include\SDL2\SDL_ttf.h" (
        echo Extracting SDL2_ttf...
        powershell -NoProfile -Command "Expand-Archive -Path SDL2_ttf-2.24.0.zip -DestinationPath . -Force"
        
        echo Copying SDL2_ttf files...
        xcopy SDL2_ttf-2.24.0\include\*.h ..\installed\x64-windows\include\SDL2\ /Y /I
        xcopy SDL2_ttf-2.24.0\lib\x64\*.lib ..\installed\x64-windows\lib\ /Y
        xcopy SDL2_ttf-2.24.0\lib\x64\*.dll ..\installed\x64-windows\bin\ /Y
    )
    
    cd ..\..
    echo SDL2_ttf already installed
)

REM Step 4: Create build directory
echo [4/5] Creating build directory...
if not exist "build" (
    mkdir build
)

REM Step 5: Copy DLLs to build directory
echo [5/5] Copying SDL2 libraries...
Copy-Item -Path "vcpkg\installed\x64-windows\bin\*.dll" -Destination "build\" -Force -ErrorAction SilentlyContinue

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
endlocal
