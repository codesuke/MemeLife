@echo off
REM MemeLife SDL2 Project - Verify Installation Script
REM This script checks that everything is set up correctly

setlocal enabledelayedexpansion
cd /d "%~dp0"

echo.
echo ============================================================
echo   MemeLife SDL2 Project - Installation Verification
echo ============================================================
echo.

set "ERRORS=0"

REM Check Git
echo [1/7] Checking Git...
where git >nul 2>&1
if errorlevel 1 (
    echo   ERROR: Git not found in PATH
    set /a ERRORS=ERRORS+1
) else (
    for /f "tokens=3" %%A in ('git --version') do echo   OK: Git %%A found
)

REM Check vcpkg
echo [2/7] Checking vcpkg...
if not exist "vcpkg" (
    echo   ERROR: vcpkg directory not found
    set /a ERRORS=ERRORS+1
) else (
    echo   OK: vcpkg directory exists
)

REM Check SDL2
echo [3/7] Checking SDL2 libraries...
if not exist "vcpkg\installed\x64-windows\lib\SDL2.lib" (
    echo   ERROR: SDL2.lib not found
    set /a ERRORS=ERRORS+1
) else (
    echo   OK: SDL2.lib found
)

if not exist "vcpkg\installed\x64-windows\include\SDL2.h" (
    echo   ERROR: SDL2.h not found
    set /a ERRORS=ERRORS+1
) else (
    echo   OK: SDL2.h found
)

REM Check SDL2_image
echo [4/7] Checking SDL2_image...
if not exist "vcpkg\installed\x64-windows\lib\SDL2_image.lib" (
    echo   ERROR: SDL2_image.lib not found
    set /a ERRORS=ERRORS+1
) else (
    echo   OK: SDL2_image.lib found
)

REM Check SDL2_mixer
echo [5/7] Checking SDL2_mixer...
if not exist "vcpkg\installed\x64-windows\lib\SDL2_mixer.lib" (
    echo   ERROR: SDL2_mixer.lib not found
    set /a ERRORS=ERRORS+1
) else (
    echo   OK: SDL2_mixer.lib found
)

REM Check SDL2_ttf
echo [6/7] Checking SDL2_ttf...
if not exist "vcpkg\installed\x64-windows\lib\SDL2_ttf.lib" (
    echo   ERROR: SDL2_ttf.lib not found
    set /a ERRORS=ERRORS+1
) else (
    echo   OK: SDL2_ttf.lib found
)

REM Check source files
echo [7/7] Checking source code...
setlocal enabledelayedexpansion
set "SRC_COUNT=0"
for /f %%A in ('dir /b src\*.cpp 2^>nul ^| find /c /v ""') do set SRC_COUNT=%%A
if !SRC_COUNT! equ 0 (
    echo   ERROR: No source files found
    set /a ERRORS=ERRORS+1
) else (
    echo   OK: !SRC_COUNT! source files found
)

echo.
echo ============================================================
if !ERRORS! equ 0 (
    echo   All checks passed! Ready to build.
    echo   Run: build.bat
) else (
    echo   !ERRORS! error(s) found. Please run setup.bat first.
    echo   Run: setup.bat
)
echo ============================================================
echo.
pause
