# MemeLife: The Game
*A college life simulation RPG built with SDL2*

## Quick Start (5 minutes)

### Windows (Recommended)

**Option 1: One-Click Setup (Easiest)**
```batch
powershell -ExecutionPolicy Bypass -File setup.ps1
```
Then run:
```batch
build.bat
build\MemeLifeSDL2.exe
```

**Option 2: Manual Batch Script**
```batch
setup.bat
build.bat
build\MemeLifeSDL2.exe
```

---

## What is MemeLife?

MemeLife is a college life simulation game where you navigate your character through university. Make choices, manage your stats (Clout, Logic, Sanity, Money, Energy), interact with NPCs, and survive to graduation.

### Current Features (Phase 4 Complete)

- **Character Creation**: Choose your origin (Indian state) for unique buffs/debuffs
- **Dynamic Stats System**: Clout, Logic, Sanity, Money, Energy with real-time tracking
- **Time System**: In-game day/hour tracking with energy drain mechanics
- **Leveling System**: Gain XP every 30 in-game days, increase age and stats
- **Event System**: Trigger events via text files, modify game state dynamically
- **HUD Display**: Real-time stat and energy bar display
- **Game Over Mechanic**: 5 strikes and you're out (hospitalization system)
- **Tile-Based World**: Navigate through an interactive 2D world

---

## Project Structure

```
MemeLife/
├── src/                    # C++ source code
│   ├── main.cpp           # Main game loop with state machine
│   ├── Player.h/cpp       # Player class with stats & leveling
│   ├── Map.h/cpp          # Tile-based world
│   ├── GameManager.h/cpp  # Game state management
│   ├── TimeManager.h/cpp  # In-game time & leveling
│   ├── EventManager.h/cpp # Event system with file parsing
│   ├── UIManager.h/cpp    # UI coordination
│   ├── HUDManager.h/cpp   # Stats display
│   ├── Buff.h/cpp         # Buff/debuff system
│   ├── CharacterCreationScene.h/cpp # Character creation UI
│   └── ... other components
├── assets/                 # Game resources
│   ├── sprites/           # Character sprites
│   ├── tilesets/          # Map tiles
│   ├── music/             # Background music
│   ├── fonts/             # TTF fonts
│   ├── sfx/               # Sound effects
│   └── events/            # Event definition files
├── build/                 # Compiled output (auto-generated)
├── CMakeLists.txt         # Build configuration
├── build.bat              # Quick build script
├── setup.bat              # Windows setup (batch)
├── setup.ps1              # Windows setup (PowerShell)
└── README.md              # This file
```

---

## For New Developers: Getting Started

### System Requirements

- **Windows 7 or later** (64-bit)
- **Visual Studio 2019 Build Tools** (or later)
  - Download: https://visualstudio.microsoft.com/downloads/
  - During install, select "Desktop development with C++"
- **~2 GB disk space** (for vcpkg and dependencies)
- **Git** (for version control)

### First Time Setup (Automated)

1. **Clone the repository:**
   ```bash
   git clone <repo-url>
   cd MemeLife
   ```

2. **Run setup** (choose one method):

   **Method A: PowerShell (Recommended, Windows 10+):**
   ```powershell
   powershell -ExecutionPolicy Bypass -File setup.ps1
   ```

   **Method B: Batch (Any Windows):**
   ```batch
   setup.bat
   ```

   This will:
   - ✓ Clone vcpkg package manager
   - ✓ Install SDL2 and all dependencies
   - ✓ Set up SDL2_ttf (font library)
   - ✓ Configure CMake
   - ✓ Prepare build directory

3. **Build the project:**
   ```batch
   build.bat
   ```
   Takes ~30-60 seconds on first build, ~5 seconds on rebuilds.

4. **Run the game:**
   ```batch
   build\MemeLifeSDL2.exe
   ```

---

## Development Workflow

### Daily Development

1. **Make code changes** in `src/` or **add assets** in `assets/`
2. **Build**: `build.bat` (5-30 seconds)
3. **Test**: `build\MemeLifeSDL2.exe`
4. **Repeat** until happy with changes

### Git Workflow

```bash
# Get latest changes
git pull origin main

# Create feature branch
git checkout -b feature/my-new-feature

# Make changes and build
# ...

# Commit your work
git add .
git commit -m "Add my awesome new feature"

# Push to repository
git push origin feature/my-new-feature

# Create Pull Request on GitHub
```

### Key Controls

- **Arrow Keys**: Move character around the world
- **E Key**: Advance dialogue / interact with NPCs
- **SPACE Key**: Trigger test event (hospitalization for testing)
- **ESC**: Exit game

---

## Architecture Overview

### Game State Machine
```
┌─────────────────────────────────────────┐
│   CHARACTER_CREATION                    │
│  (Pick origin, apply buffs)             │
│         ↓                               │
│   ┌─────────────────────────────┐       │
│   │    PLAYING                  │       │
│   │ (Main game loop)            │       │
│   │ Updates: Player, Map,       │       │
│   │          Time, UI           │       │
│   └─────────────────────────────┘       │
│         ↓                               │
│   GAME_OVER                             │
│  (5 hospitalizations)                   │
└─────────────────────────────────────────┘
```

### Component Architecture

Each major system is a self-contained class:

- **Player**: Stats (Clout, Logic, Sanity, Money, Energy, Level, Age), position, sprite
- **Map**: 32x24 tile grid, collision detection, rendering
- **TimeManager**: In-game clock (days, hours, minutes), leveling trigger
- **EventManager**: Text file parsing, command execution, dialogue/stat changes
- **GameManager**: Game state transitions, hospitalization tracking
- **UIManager**: Dialogue box management, HUD coordination
- **HUDManager**: Real-time stat display, energy bar with color coding
- **AudioManager**: Music looping, SFX playback
- **BuffDatabase**: Indian state origins with unique stat modifiers
- **CharacterCreationScene**: UI for selecting origin at game start

---

## Adding Content

### Create a New Event

1. **Create file**: `assets/events/EV_YOUR_EVENT.txt`
2. **Write commands**:
   ```
   DIALOGUE "This is what the player sees"
   ADD_MONEY 100
   SUBTRACT_MONEY 50
   ADD_STAT energy 20
   ADD_STAT clout 5
   ADD_STAT logic 3
   ADD_STAT sanity 10
   PLAY_SOUND notification.wav
   HOSPITALIZE
   ```
3. **Trigger in code**:
   ```cpp
   eventManager.TriggerEvent("EV_YOUR_EVENT", player, uiManager, audioManager, gameManager.get());
   ```

### Add a New Origin (Buff)

1. **Edit**: `src/Buff.cpp`
2. **Add case to `BuffDatabase::GetBuffForState()`**:
   ```cpp
   else if (stateName == "Goa") {
       return {
           "Beach Bum",
           "You grew up where life is relaxed.\n"
           "Sanity +20%, Money -10%",
           1.0f,   // cloutModifier
           1.0f,   // logicModifier
           1.2f,   // sanityModifier
           0.9f,   // moneyModifier
           1.0f,   // energyModifier
           1.0f,   // energyDrainModifier
           0, 0, 0, 0, 0.0f
       };
   }
   ```

### Modify Game Balance

Edit constants in header files:

- **Energy drain**: `src/Player.h` → `ENERGY_DRAIN_PER_HOUR = 4.0f`
- **Level speed**: `src/TimeManager.h` → `DAYS_PER_LEVEL = 30`
- **Hospitalization limit**: `src/GameManager.h` → `MAX_HOSPITALIZATIONS = 5`
- **Game window size**: `src/main.cpp` → `WINDOW_WIDTH/HEIGHT`

---

## Troubleshooting

### Build Issues

| Error | Solution |
|-------|----------|
| "cmake not found" | Run `setup.ps1` again - it downloads CMake automatically |
| "ninja: command not found" | Delete `build/` folder and run `setup.ps1` |
| "SDL2.h: No such file" | Run `setup.ps1` - may need to reinstall dependencies |
| DLL not found at runtime | Run `setup.ps1` or manually copy DLLs from `vcpkg/installed/x64-windows/bin/` to `build/` |
| "vcpkg installation failed" | Delete `vcpkg/` folder and run `setup.ps1` from scratch |

### Runtime Issues

| Error | Solution |
|-------|----------|
| "Failed to load font" | Check `assets/fonts/Jersey10-Regular.ttf` exists |
| "Failed to load sprite" | Check `assets/sprites/player.png` and `assets/tilesets/world_tileset.png` exist |
| "Failed to load music" | Check `assets/music/theme.wav` exists |
| No sound playing | Check `assets/music/` folder and verify SDL_mixer initialization |

---

## Project Phases

### Phase 1: The World Engine ✓
- Tile-based rendering
- Player movement and collision
- Map system

### Phase 2: The Interactive Layer ✓
- Dialogue system
- Time tracking
- UI framework

### Phase 3: The Simulation Engine ✓
- Character stats
- Event system
- Energy drain & leveling

### Phase 4: The RPG Framework ✓
- Character creation
- Buff/debuff system
- Leveling progression
- Hospitalization/game over

### Phase 5+: To Come
- NPC system with animations
- Save/load functionality
- Multiple locations
- Main menu
- Dialogue trees
- Inventory system

---

## Performance Tips

- **First build**: ~60 seconds (all dependencies compiled)
- **Incremental builds**: ~5 seconds (only changed files)
- **Game runtime**: 60 FPS target, smooth on any modern system

---

## Code Style

- **Naming**: camelCase for variables/methods, PascalCase for classes
- **Headers**: `#pragma once` guards
- **Memory**: Use `std::unique_ptr` for ownership, raw pointers for references
- **Comments**: Explain the "why", not the "what"

---

## Contributing

### Pull Request Checklist

- [ ] Code compiles without warnings (or acceptable warnings only)
- [ ] Game runs without crashes
- [ ] New feature is tested
- [ ] Commit messages are clear
- [ ] No hardcoded paths (use relative paths from project root)
- [ ] Assets are organized in proper folders

---

## Useful Commands

```bash
# Full rebuild
rmdir /s build
build.bat

# Just compile (no CMake)
cd build
ninja

# Run with output
build\MemeLifeSDL2.exe 2>&1

# Show all source files
dir /s src\*.cpp src\*.h
```

---

## License

MIT License - You're free to use, modify, and distribute this project.

---

## Getting Help

1. **Check README** - you're reading it!
2. **Check code comments** - especially in main.cpp
3. **Look at similar examples** - other classes follow same patterns
4. **Ask in issues** - describe problem + what you've tried
5. **Discord/Slack** - ask team members in dev channel

---

## Performance Monitoring

Add this to main.cpp to see FPS:
```cpp
static int frameCount = 0;
static Uint32 lastTime = SDL_GetTicks();
frameCount++;
if (SDL_GetTicks() - lastTime >= 1000) {
    std::cout << "FPS: " << frameCount << std::endl;
    frameCount = 0;
    lastTime = SDL_GetTicks();
}
```

---

**Last Updated**: November 8, 2025  
**Version**: Phase 4 (Complete)  
**Status**: Ready for Phase 5 Development  
**Lead Developer**: [Your Name Here]


### ✅ Step 1.4: Player & Movement System
- `Player` class with sprite loading
- Arrow key controls (UP, DOWN, LEFT, RIGHT)
- Smooth movement with `SPEED = 4.0` pixels per frame
- Player rendering on the map

### ✅ Step 1.5: Collision Detection & Audio
- `Map::IsSolidTileAt()` function for collision checks
- Player movement blocked by walls
- Separate collision checks for horizontal and vertical movement (prevents corner-sticking)
- `AudioManager` class for background music
- Looping background music playback

---

## How to Build & Run

### Build the Project
```bash
cd "c:\Study\Programming\PROJECTS\College Projects\MemeLife\SDL2 Version"
.\build.bat
```

### Run the Game
```bash
cd build
.\MemeLifeSDL2.exe
```

**Controls:**
- **Arrow Keys** - Move player around
- **ESC** - Quit game
- **Click X** - Close window

---

## Project Structure

```
SDL2 Version/
├── CMakeLists.txt
├── build.bat                     # Build script
├── generate_assets.py            # Asset generator
├── generate_audio.py             # Audio generator
├── src/
│   ├── main.cpp                 # Main game loop
│   ├── Map.h / Map.cpp          # Tile-based map system
│   ├── Player.h / Player.cpp    # Player entity
│   └── AudioManager.h / AudioManager.cpp  # Audio system
├── assets/
│   ├── sprites/
│   │   └── player.png           # 32x32 player sprite
│   ├── tilesets/
│   │   └── world_tileset.png    # 64x32 (floor + wall tiles)
│   └── music/
│       ├── theme.wav            # Background music
│       └── theme.mp3            # (Optional MP3, requires additional SDL2_mixer codecs)
├── build/                        # Build output directory
└── vcpkg/                        # Package manager (SDL2, SDL2_image, SDL2_mixer)

```

---

## Classes & Key Components

### `Map` Class
- **Members**: 2D grid of tile data, tileset texture
- **Methods**: `Render()`, `IsSolidTileAt(x, y)`
- **Features**: 32x32 pixel tiles, wall collision detection

### `Player` Class
- **Members**: Sprite texture, position (float x, y)
- **Methods**: `Update()`, `Render()`, `GetX()`, `GetY()`
- **Features**: Arrow key movement, collision-aware pathfinding

### `AudioManager` Class
- **Members**: Current music pointer, initialization state
- **Methods**: `Initialize()`, `PlayMusic()`, `StopMusic()`, `SetMusicVolume()`
- **Features**: Looping background music, error handling

---

## Verification Results

✅ Window opens and closes properly  
✅ Tile map renders with floor and wall tiles  
✅ Player sprite is visible and centered on map  
✅ Player moves with arrow keys in all 4 directions  
✅ Player is stopped by wall tiles (no clipping through walls)  
✅ Background music plays on loop  
✅ Game runs at 60 FPS  
✅ ESC and X button exit cleanly  

---

## Next Steps (Phase 2)

Once you're ready to continue, Phase 2 will include:
- NPCs with dialogue system
- Inventory system
- Items and interactable objects
- Enhanced visual effects and animations
- Advanced collision/trigger zones
- Expanding the world with multiple rooms

---

## Technology Stack

- **Language**: C++17
- **Build System**: CMake 3.30+
- **Compiler**: MSVC 19.29 (Visual Studio 2019 BuildTools)
- **Graphics**: SDL2 2.32.10
- **Image Loading**: SDL2_image 2.8.8
- **Audio**: SDL2_mixer 2.8.1
- **Package Manager**: vcpkg

---

## Notes

- Asset paths use relative paths from the `build/` directory (`../assets/`)
- Music is in WAV format (universally supported by SDL2_mixer)
- Game runs with vsync enabled for smooth 60 FPS rendering
- All classes properly handle resource cleanup (textures, window, renderer)

---

**Status**: 🟢 Phase 1 Complete - Ready for Phase 2 Development!
