#!/usr/bin/env powershell

# MemeLife SDL2 Project - Setup Script (PowerShell)
# Run with: powershell -ExecutionPolicy Bypass -File setup.ps1

Write-Host "============================================================" -ForegroundColor Cyan
Write-Host "  MemeLife SDL2 Project - Setup Wizard" -ForegroundColor Cyan
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host ""

$ProjectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $ProjectRoot

# Step 1: Clone vcpkg
Write-Host "[1/5] Checking vcpkg..." -ForegroundColor Yellow
if (-not (Test-Path "vcpkg")) {
    Write-Host "Cloning vcpkg repository..." -ForegroundColor Green
    git clone https://github.com/Microsoft/vcpkg.git
    & "vcpkg\bootstrap-vcpkg.bat" -ErrorAction Stop
} else {
    Write-Host "vcpkg already exists" -ForegroundColor Green
}

# Step 2: Install SDL2 packages
Write-Host "[2/5] Installing SDL2 packages..." -ForegroundColor Yellow
if (-not (Test-Path "vcpkg\installed\x64-windows\lib\SDL2.lib")) {
    Write-Host "Installing SDL2 and dependencies..." -ForegroundColor Green
    & "vcpkg\vcpkg.exe" install `
        SDL2:x64-windows `
        SDL2-image:x64-windows `
        SDL2-mixer:x64-windows `
        -ErrorAction Stop
} else {
    Write-Host "SDL2 packages already installed" -ForegroundColor Green
}

# Step 3: Setup SDL2_ttf
Write-Host "[3/5] Setting up SDL2_ttf..." -ForegroundColor Yellow
$ttfHeader = "vcpkg\installed\x64-windows\include\SDL2\SDL_ttf.h"
if (-not (Test-Path $ttfHeader)) {
    Write-Host "Downloading SDL2_ttf 2.24.0..." -ForegroundColor Green
    
    $dlUrl = "https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.24.0/SDL2_ttf-devel-2.24.0-VC.zip"
    $dlPath = "vcpkg\downloads\SDL2_ttf-2.24.0.zip"
    
    if (-not (Test-Path $dlPath)) {
        New-Item -ItemType Directory -Path "vcpkg\downloads" -Force | Out-Null
        Invoke-WebRequest -Uri $dlUrl -OutFile $dlPath
    }
    
    Write-Host "Extracting SDL2_ttf..." -ForegroundColor Green
    Expand-Archive -Path $dlPath -DestinationPath "vcpkg\downloads" -Force
    
    Write-Host "Copying SDL2_ttf files..." -ForegroundColor Green
    $ttfDir = "vcpkg\downloads\SDL2_ttf-2.24.0"
    Copy-Item "$ttfDir\include\*.h" "vcpkg\installed\x64-windows\include\SDL2\" -Force
    Copy-Item "$ttfDir\lib\x64\*.lib" "vcpkg\installed\x64-windows\lib\" -Force
    Copy-Item "$ttfDir\lib\x64\*.dll" "vcpkg\installed\x64-windows\bin\" -Force
} else {
    Write-Host "SDL2_ttf already installed" -ForegroundColor Green
}

# Step 4: Create build directory
Write-Host "[4/5] Preparing build directory..." -ForegroundColor Yellow
if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" -Force | Out-Null
    Write-Host "Build directory created" -ForegroundColor Green
} else {
    Write-Host "Cleaning build directory..." -ForegroundColor Green
    Remove-Item "build\*" -Recurse -Force -ErrorAction SilentlyContinue
}

# Step 5: Run CMake
Write-Host "[5/5] Running CMake configuration..." -ForegroundColor Yellow
Push-Location "build"
$cmake = "..\vcpkg\downloads\tools\cmake-3.30.1-windows\cmake-3.30.1-windows-i386\bin\cmake.exe"
$ninja = "..\vcpkg\downloads\tools\ninja\1.13.1-windows\ninja.exe"
& $cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM="$ninja"
Pop-Location

Write-Host ""
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host "  Setup Complete! You're ready to build MemeLife!" -ForegroundColor Cyan
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Yellow
Write-Host "  1. Run 'build.bat' to compile the project" -ForegroundColor White
Write-Host "  2. Run 'build\MemeLifeSDL2.exe' to start the game" -ForegroundColor White
Write-Host ""
Write-Host "For developers:" -ForegroundColor Yellow
Write-Host "  - Edit code in 'src/' directory" -ForegroundColor White
Write-Host "  - Assets go in 'assets/' directory" -ForegroundColor White
Write-Host "  - Run 'build.bat' after making changes" -ForegroundColor White
Write-Host ""
