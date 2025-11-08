#!/usr/bin/env powershell

# MemeLife SDL2 Project - Cross-Platform Setup Script (PowerShell)
# Run with: powershell -ExecutionPolicy Bypass -File setup.ps1

Write-Host "============================================================" -ForegroundColor Cyan
Write-Host "  MemeLife SDL2 Project - Setup Wizard" -ForegroundColor Cyan
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host ""

$ProjectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $ProjectRoot

# Function to find Visual Studio
function Find-VisualStudio {
    $vsVersions = @("2022", "2019", "2017")
    $vsPaths = @(
        "C:\Program Files\Microsoft Visual Studio",
        "C:\Program Files (x86)\Microsoft Visual Studio"
    )
    
    foreach ($basePath in $vsPaths) {
        if (Test-Path $basePath) {
            foreach ($version in $vsVersions) {
                $versionPath = Join-Path $basePath $version
                if (Test-Path $versionPath) {
                    $editions = Get-ChildItem -Path $versionPath -Directory
                    foreach ($edition in $editions) {
                        $vcvars = Join-Path $edition.FullName "VC\Auxiliary\Build\vcvarsall.bat"
                        if (Test-Path $vcvars) {
                            return $vcvars
                        }
                    }
                }
            }
        }
    }
    return $null
}

# Function to find CMake
function Find-CMake {
    $cmakePaths = Get-ChildItem -Path "vcpkg\downloads\tools\cmake-*" -Directory -ErrorAction SilentlyContinue
    if ($cmakePaths) {
        $latest = $cmakePaths | Sort-Object Name -Descending | Select-Object -First 1
        $cmake = Join-Path $latest.FullName "bin\cmake.exe"
        if (Test-Path $cmake) {
            return $cmake
        }
    }
    return $null
}

# Function to find Ninja
function Find-Ninja {
    $ninjaPaths = Get-ChildItem -Path "vcpkg\downloads\tools\ninja\*" -Directory -ErrorAction SilentlyContinue
    if ($ninjaPaths) {
        $latest = $ninjaPaths | Sort-Object Name -Descending | Select-Object -First 1
        $ninja = Join-Path $latest.FullName "ninja.exe"
        if (Test-Path $ninja) {
            return $ninja
        }
    }
    return $null
}

# Check for Git
Write-Host "[Check] Verifying Git installation..." -ForegroundColor Yellow
if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
    Write-Host "Error: Git is not installed or not in PATH" -ForegroundColor Red
    Write-Host "Please install Git from https://git-scm.com/download/win" -ForegroundColor Red
    exit 1
}
Write-Host "Git found" -ForegroundColor Green

# Step 1: Clone vcpkg
Write-Host "[1/5] Checking vcpkg..." -ForegroundColor Yellow
if (-not (Test-Path "vcpkg")) {
    Write-Host "Cloning vcpkg repository..." -ForegroundColor Green
    & git clone https://github.com/Microsoft/vcpkg.git
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Error: Failed to clone vcpkg" -ForegroundColor Red
        exit 1
    }
    
    Write-Host "Bootstrapping vcpkg..." -ForegroundColor Green
    & "vcpkg\bootstrap-vcpkg.bat"
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Error: Failed to bootstrap vcpkg" -ForegroundColor Red
        exit 1
    }
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
        SDL2-mixer:x64-windows
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Error: Failed to install SDL2 packages" -ForegroundColor Red
        exit 1
    }
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
        New-Item -ItemType Directory -Path "vcpkg\downloads" -Force -ErrorAction SilentlyContinue | Out-Null
        try {
            Invoke-WebRequest -Uri $dlUrl -OutFile $dlPath -ErrorAction Stop
        } catch {
            Write-Host "Error: Failed to download SDL2_ttf" -ForegroundColor Red
            Write-Host $_.Exception.Message -ForegroundColor Red
            exit 1
        }
    }
    
    Write-Host "Extracting SDL2_ttf..." -ForegroundColor Green
    Expand-Archive -Path $dlPath -DestinationPath "vcpkg\downloads" -Force
    
    Write-Host "Copying SDL2_ttf files..." -ForegroundColor Green
    $ttfDir = "vcpkg\downloads\SDL2_ttf-2.24.0"
    Copy-Item "$ttfDir\include\*.h" "vcpkg\installed\x64-windows\include\SDL2\" -Force -ErrorAction SilentlyContinue
    Copy-Item "$ttfDir\lib\x64\*.lib" "vcpkg\installed\x64-windows\lib\" -Force -ErrorAction SilentlyContinue
    Copy-Item "$ttfDir\lib\x64\*.dll" "vcpkg\installed\x64-windows\bin\" -Force -ErrorAction SilentlyContinue
} else {
    Write-Host "SDL2_ttf already installed" -ForegroundColor Green
}

# Step 4: Create build directory
Write-Host "[4/5] Preparing build directory..." -ForegroundColor Yellow
if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" -Force -ErrorAction SilentlyContinue | Out-Null
    Write-Host "Build directory created" -ForegroundColor Green
} else {
    Write-Host "Build directory exists" -ForegroundColor Green
}

# Step 5: Copy DLLs
Write-Host "[5/5] Copying SDL2 libraries..." -ForegroundColor Yellow
Copy-Item -Path "vcpkg\installed\x64-windows\bin\*.dll" -Destination "build\" -Force -ErrorAction SilentlyContinue
Write-Host "Libraries copied" -ForegroundColor Green

Write-Host ""
Write-Host "============================================================" -ForegroundColor Green
Write-Host "  Setup Complete!" -ForegroundColor Green
Write-Host "============================================================" -ForegroundColor Green
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Yellow
Write-Host "  1. Run 'build.bat' or './build.bat' to compile the project" -ForegroundColor White
Write-Host "  2. Run '.\build\MemeLifeSDL2.exe' to start the game" -ForegroundColor White
Write-Host ""
Write-Host "For development on this system:" -ForegroundColor Yellow

$vsPath = Find-VisualStudio
if ($vsPath) {
    Write-Host "  Visual Studio: FOUND" -ForegroundColor Green
} else {
    Write-Host "  Visual Studio: NOT FOUND - Please install Visual Studio Build Tools" -ForegroundColor Red
    Write-Host "  Download from: https://visualstudio.microsoft.com/downloads/" -ForegroundColor Yellow
}

$cmakePath = Find-CMake
if ($cmakePath) {
    Write-Host "  CMake: FOUND at $cmakePath" -ForegroundColor Green
} else {
    Write-Host "  CMake: Will be auto-detected during build" -ForegroundColor Yellow
}

$ninjaPath = Find-Ninja
if ($ninjaPath) {
    Write-Host "  Ninja: FOUND at $ninjaPath" -ForegroundColor Green
} else {
    Write-Host "  Ninja: Will be auto-detected during build" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "Troubleshooting:" -ForegroundColor Yellow
Write-Host "  - If Visual Studio is not found, it will be auto-detected during build" -ForegroundColor Gray
Write-Host "  - If CMake or Ninja is not found, vcpkg will provide them automatically" -ForegroundColor Gray
Write-Host "  - For issues, check README.md or CONTRIBUTING.md" -ForegroundColor Gray
Write-Host ""
