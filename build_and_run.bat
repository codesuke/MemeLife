@echo off
echo ================================
echo MemeLife - Quick Build and Run
echo ================================
echo.

echo Compiling MemeLife...
g++ -std=c++11 -Iinclude src/main.cpp src/Terminal.cpp src/AssetLoader.cpp -o MemeLife.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ✓ Build successful!
    echo.
    echo Starting MemeLife...
    echo.
    MemeLife.exe
) else (
    echo.
    echo ✗ Build failed. Please check for errors above.
    pause
)
