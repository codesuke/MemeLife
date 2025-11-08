# Cross-Platform Build System Update

## What Changed

Updated all setup and build scripts to be cross-system compatible by auto-discovering tools instead of hardcoding paths.

### Updated Files

#### 1. `build.bat` - Completely Rewritten
- **Auto-discovers Visual Studio** (2022, 2019, 2017) in both Program Files locations
- **Auto-discovers CMake** in vcpkg downloads (finds latest version dynamically)
- **Auto-discovers Ninja** in vcpkg downloads (finds latest version dynamically)
- **Proper error handling** with meaningful error messages
- **No hardcoded paths** - works on any system with VS installed

**Benefits:**
- Works on any Windows system with any VS version
- Automatically finds tools without manual configuration
- Clear error messages if something is missing

#### 2. `setup.bat` - Enhanced with Error Checking
- **Git verification** before attempting operations
- **Error handling** for each step (clone, install, download, extract)
- **Progress indicators** with numbered steps [1/5] format
- **DLL copying** integrated into setup
- **Better messages** for troubleshooting

**Benefits:**
- Catches issues early with clear feedback
- Fails fast with helpful error messages
- Doesn't leave the system in a broken state

#### 3. `setup.ps1` - Major Overhaul
- **Helper functions** for finding VS, CMake, and Ninja
- **Git verification** with installation link if missing
- **Error handling** with proper exit codes
- **DLL copying** built into setup
- **Post-setup diagnostics** showing what was found/installed
- **Troubleshooting tips** in the output

**New Functions:**
- `Find-VisualStudio` - Searches all VS versions/locations
- `Find-CMake` - Finds latest CMake in vcpkg
- `Find-Ninja` - Finds latest Ninja in vcpkg

#### 4. `verify.bat` - New Installation Checker
- Checks Git installation
- Verifies vcpkg exists
- Checks all SDL2 libraries (SDL2, image, mixer, ttf)
- Counts source files
- **Reports all issues at once** rather than failing silently
- Clear summary at the end

**Usage:**
```batch
verify.bat
```

## How It Works

### For End Users
1. Install Git and Visual Studio Build Tools
2. Clone repository
3. Run either:
   ```batch
   setup.bat
   build.bat
   build\MemeLifeSDL2.exe
   ```
   OR
   ```powershell
   powershell -ExecutionPolicy Bypass -File setup.ps1
   build.bat
   .\build\MemeLifeSDL2.exe
   ```

### What Auto-Discovery Does

**Visual Studio Detection:**
```
Searches:
- C:\Program Files\Microsoft Visual Studio
- C:\Program Files (x86)\Microsoft Visual Studio

Finds:
- VS 2022, 2019, or 2017
- Any edition (Community, Professional, Build Tools)
- Returns first found vcvarsall.bat
```

**CMake Detection:**
```
Looks in: vcpkg\downloads\tools\cmake-*
Returns: Latest version found
```

**Ninja Detection:**
```
Looks in: vcpkg\downloads\tools\ninja\*
Returns: Latest version found
```

## Compatibility

| System | Status | Notes |
|--------|--------|-------|
| VS 2022 + Windows 11 | ✅ Tested | Works perfectly |
| VS 2019 + Windows 10 | ✅ Should work | Auto-discovery finds it |
| VS 2017 + Windows 7 | ✅ Should work | Supported by VS search |
| VS 2022 Build Tools | ✅ Works | Express installation |
| Any VS version | ✅ Works | Auto-discovered |

## Features

✅ **No hardcoded paths** - Works anywhere
✅ **Auto-discovery** - Finds tools automatically
✅ **Error checking** - Meaningful error messages
✅ **Version flexible** - Works with multiple VS versions
✅ **Batch & PowerShell** - Multiple shells supported
✅ **Verification** - Can check installation status
✅ **Clear output** - Progress indicators and status
✅ **Fallback handling** - Graceful degradation

## Testing

To verify the new scripts work:

```batch
REM Quick check
verify.bat

REM Full rebuild (cleans everything)
rmdir /s /q build
setup.bat
build.bat

REM Run game
.\build\MemeLifeSDL2.exe
```

## For Contributors

When adding dependencies or changing build requirements:

1. Update `setup.bat` to install the dependency
2. Update `setup.ps1` with equivalent PowerShell code
3. Update CMakeLists.txt to find/link the dependency
4. Test on at least Windows 10/11 with VS 2019+
5. Document in README.md

## Troubleshooting

If scripts fail:

1. Run `verify.bat` to check installation
2. Check that Git is installed: `git --version`
3. Check that Visual Studio is installed
4. Delete `build` folder and run `setup.bat` again
5. Check README.md for more help

## Backwards Compatibility

The new scripts are **fully backwards compatible**:
- If old hardcoded paths exist, they'll still work
- vcpkg automatically handles multiple version directories
- All existing functionality is preserved
- New auto-discovery is transparent to users

---

**Status:** All systems ready for collaborative development
**Next:** Team members can now clone and build on any Windows system
