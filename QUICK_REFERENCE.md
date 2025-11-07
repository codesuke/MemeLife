# 🚀 MemeLife - Quick Reference

## Build Commands

### Quick Build (Windows)
```powershell
.\build_and_run.bat
```

### Manual Compile
```powershell
g++ -std=c++11 -Iinclude src/main.cpp src/Terminal.cpp src/AssetLoader.cpp -o MemeLife
```

### Using Make
```bash
make           # Build
make clean     # Clean build files
make run       # Build and run
```

### Using CMake
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

---

## File Locations

| What | Where |
|------|-------|
| Source code | `src/*.cpp` |
| Headers | `include/*.h` |
| Events | `assets/events/*.txt` |
| Docs | `docs/*.md` |
| Instructions | `Instructions/*.md` |
| Build configs | `CMakeLists.txt`, `Makefile` |

---

## Common Tasks

### Create New Event
```
1. Create: assets/events/XX_name.txt
2. Add content:
   ID: XX_name
   DESC: Event description here
   CHOICE: Choice text | next_event_id
3. Link from another event's CHOICE line
```

### Add New Source File
```
1. Create: src/NewModule.cpp
2. Create: include/NewModule.h
3. Update: CMakeLists.txt (add to SOURCES)
4. Update: Makefile (add to SOURCES)
5. Include in main.cpp: #include "NewModule.h"
```

### Debug Issues
```powershell
# Rebuild from scratch
make clean
g++ -std=c++11 -Iinclude src/*.cpp -o MemeLife

# Check event file exists
Test-Path "assets/events/01_start.txt"

# Verify compilation
g++ --version
```

---

## Event File Format

```
ID: unique_event_id
DESC: Single line description shown to player.
CHOICE: First choice text | event_id_1
CHOICE: Second choice text | event_id_2
ART: Optional ASCII art line
```

**Rules:**
- ID must match filename (without .txt)
- DESC is single line (use . for pauses)
- CHOICE uses | to separate text from outcome ID
- Each event needs at least one CHOICE

---

## Key Files

### `src/main.cpp`
- Game class
- Main game loop
- Entry point

### `include/GameData.h`
- Stats struct
- Player struct
- Choice struct
- GameEvent struct

### `src/Terminal.cpp`
- ClearScreen()
- MoveCursor(x, y)
- SetColor(code)
- GetImmediateInput()

### `src/AssetLoader.cpp`
- LoadEvent(eventId)
- Parses event text files

---

## Controls

| Key | Action |
|-----|--------|
| Any key | Continue/Advance |
| q | Quit game |
| 1-9 | Select choice (Phase 2+) |

---

## Project Structure

```
MemeLife/
├── src/              Source files
├── include/          Headers
├── assets/events/    Event files
├── docs/             Documentation
├── Instructions/     Phase guides
└── .vscode/          VS Code config
```

---

## Troubleshooting

### "g++ not recognized"
- Install MinGW-w64
- Add to PATH: `C:\MinGW\bin`

### "No such file or directory"
- Check you're in project root
- Verify file paths are correct

### "Event not loading"
- Check filename matches ID
- Verify file is in `assets/events/`
- Check file format (ID:, DESC:, CHOICE:)

### Compilation errors
- Check all #include statements
- Verify all .cpp files are listed in build command
- Ensure C++11 standard is set

---

## VS Code Commands

| Command | Action |
|---------|--------|
| `Ctrl + Shift + B` | Build |
| `F5` | Debug |
| `Ctrl + ~` | Toggle terminal |

---

## Phase Status

- ✅ Phase 1: Core Engine
- 🔜 Phase 2: Enhanced Gameplay
- 📋 Phase 3: Advanced Mechanics
- 📋 Phase 4: Polish & Features

---

**For detailed info, see:**
- `README.md` - Main documentation
- `docs/PROJECT_OVERVIEW.md` - Architecture details
- `docs/PHASE1_COMPLETE.md` - Phase 1 completion notes
- `Instructions/Phase*.md` - Development guides
