# Phase 1 Verification Script
# This script checks if all Phase 1 requirements are completed

Write-Host "=== MemeLife Phase 1 Verification ===" -ForegroundColor Cyan
Write-Host ""

$allPassed = $true

# Step 1.1: Check folder structure
Write-Host "Step 1.1: Project Structure" -ForegroundColor Yellow
$folders = @("src", "include", "assets", "assets\events")
foreach ($folder in $folders)
{
    if (Test-Path $folder)
    {
        Write-Host "  checkmark $folder folder exists" -ForegroundColor Green
    }
    else
    {
        Write-Host "  X $folder folder missing" -ForegroundColor Red
        $allPassed = $false
    }
}

# Check for build files
if (Test-Path "CMakeLists.txt")
{
    Write-Host "  checkmark CMakeLists.txt exists" -ForegroundColor Green
}
else
{
    Write-Host "  X CMakeLists.txt missing" -ForegroundColor Red
    $allPassed = $false
}

if (Test-Path "Makefile")
{
    Write-Host "  checkmark Makefile exists" -ForegroundColor Green
}
else
{
    Write-Host "  X Makefile missing" -ForegroundColor Red
    $allPassed = $false
}

Write-Host ""

# Step 1.2: Check Terminal module
Write-Host "Step 1.2: Terminal Control Module" -ForegroundColor Yellow
$terminalFiles = @("include\Terminal.h", "src\Terminal.cpp")
foreach ($file in $terminalFiles)
{
    if (Test-Path $file)
    {
        Write-Host "  checkmark $file exists" -ForegroundColor Green
    }
    else
    {
        Write-Host "  X $file missing" -ForegroundColor Red
        $allPassed = $false
    }
}

Write-Host ""

# Step 1.3: Check GameData structures
Write-Host "Step 1.3: Core Data Structures" -ForegroundColor Yellow
if (Test-Path "include\GameData.h")
{
    Write-Host "  checkmark GameData.h exists" -ForegroundColor Green
    $content = Get-Content "include\GameData.h" -Raw
    $structs = @("Stats", "Player", "Choice", "GameEvent")
    foreach ($struct in $structs)
    {
        if ($content -match "struct $struct")
        {
            Write-Host "  checkmark $struct struct defined" -ForegroundColor Green
        }
        else
        {
            Write-Host "  X $struct struct missing" -ForegroundColor Red
            $allPassed = $false
        }
    }
}
else
{
    Write-Host "  X GameData.h missing" -ForegroundColor Red
    $allPassed = $false
}

Write-Host ""

# Step 1.4: Check Game class and main.cpp
Write-Host "Step 1.4: Game Loop and State Manager" -ForegroundColor Yellow
if (Test-Path "src\main.cpp")
{
    Write-Host "  checkmark main.cpp exists" -ForegroundColor Green
    $content = Get-Content "src\main.cpp" -Raw
    if ($content -match "class Game")
    {
        Write-Host "  checkmark Game class defined" -ForegroundColor Green
    }
    else
    {
        Write-Host "  X Game class missing" -ForegroundColor Red
        $allPassed = $false
    }
    if ($content -match "while.*isRunning")
    {
        Write-Host "  checkmark Game loop implemented" -ForegroundColor Green
    }
    else
    {
        Write-Host "  X Game loop missing" -ForegroundColor Red
        $allPassed = $false
    }
}
else
{
    Write-Host "  X main.cpp missing" -ForegroundColor Red
    $allPassed = $false
}

Write-Host ""

# Step 1.5: Check AssetLoader
Write-Host "Step 1.5: Asset Parser" -ForegroundColor Yellow
$assetFiles = @("include\AssetLoader.h", "src\AssetLoader.cpp", "assets\events\01_start.txt")
foreach ($file in $assetFiles)
{
    if (Test-Path $file)
    {
        Write-Host "  checkmark $file exists" -ForegroundColor Green
    }
    else
    {
        Write-Host "  X $file missing" -ForegroundColor Red
        $allPassed = $false
    }
}

Write-Host ""

# Check if executable can be built
Write-Host "Build Test" -ForegroundColor Yellow
if (Test-Path "MemeLife.exe")
{
    Write-Host "  checkmark MemeLife.exe built successfully" -ForegroundColor Green
}
else
{
    Write-Host "  ! MemeLife.exe not found (run build command first)" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "================================" -ForegroundColor Cyan
if ($allPassed)
{
    Write-Host "checkmark All Phase 1 requirements completed!" -ForegroundColor Green
    Write-Host ""
    Write-Host "Next steps:" -ForegroundColor Cyan
    Write-Host "  1. Test the game: .\MemeLife.exe"
    Write-Host "  2. Add more event files to assets\events\"
    Write-Host "  3. Proceed to Phase 2"
}
else
{
    Write-Host "X Some requirements are missing. Please review above." -ForegroundColor Red
}
Write-Host "================================" -ForegroundColor Cyan
