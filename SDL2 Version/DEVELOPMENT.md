# Development Guide - MemeLife SDL2

## Setup Instructions (First Time Only)

### 1. Install Dependencies via vcpkg
```bash
cd "c:\Study\Programming\PROJECTS\College Projects\MemeLife\SDL2 Version"
.\vcpkg\vcpkg install sdl2 sdl2-image sdl2-mixer
```

### 2. Generate Assets (if needed)
```bash
python generate_assets.py    # Creates player sprite and tileset
python generate_audio.py     # Creates background music
```

---

## Daily Development Workflow

### Building the Project
```bash
.\build.bat
```
This script:
1. Sets up the Visual Studio 2019 environment
2. Runs CMake to configure the build
3. Compiles with Ninja build system
4. Links all libraries (SDL2, SDL2_image, SDL2_mixer)

### Running the Game
```bash
cd build
.\MemeLifeSDL2.exe
```

### Quick Build & Run
```bash
.\build.bat && cd build && .\MemeLifeSDL2.exe
```

---

## Project Architecture

### Core Game Loop (main.cpp)
```
Initialize SDL2 subsystems
  ↓
Load Game Objects (Map, Player, AudioManager)
  ↓
Game Loop (60 FPS):
  - Handle Events
  - Update Player
  - Clear Screen
  - Render Map
  - Render Player
  - Present to Window
  ↓
Cleanup & Exit
```

### Map System
- **Grid-based**: 32x24 tiles at 32x32 pixels each
- **Level Data**: 2D array (0=floor, 1=wall)
- **Rendering**: Double-loop through grid, copy from tileset texture

### Player System
- **Position**: Float x/y for sub-pixel accuracy
- **Movement**: Arrow keys modify position by SPEED pixels/frame
- **Collision**: Check 4 corners of player sprite before moving

### Audio System
- **Initialization**: SDL_mixer opens audio device on startup
- **Loading**: WAV files supported natively
- **Playback**: Music loops indefinitely (-1 parameter)

---

## File Modifications

### When Adding New Features:

1. **New Entities**: Create `Class.h` and `Class.cpp` in `src/`
2. **Include in CMakeLists.txt**: Add to executable sources and headers
3. **Include in main.cpp**: Add instantiation and update/render calls
4. **Rebuild**: Run `.\build.bat`

### Typical Class Structure:
```cpp
// YourClass.h
#pragma once
#include <SDL2/SDL.h>

class YourClass {
public:
    YourClass(SDL_Renderer* renderer);
    ~YourClass();
    
    void Update();  // Called each frame
    void Render();  // Called each frame to draw
    
private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    // ... other data
};

// YourClass.cpp
#include "YourClass.h"

YourClass::YourClass(SDL_Renderer* renderer)
    : renderer(renderer), texture(nullptr) {
    // Initialize
}

YourClass::~YourClass() {
    if (texture) SDL_DestroyTexture(texture);
}

void YourClass::Update() { /* ... */ }
void YourClass::Render() { /* ... */ }
```

---

## Asset Management

### Adding Player Sprites
1. Create 32x32 PNG image
2. Place in `assets/sprites/`
3. Update `main.cpp` player loading path
4. Rebuild

### Adding Tiles
1. Create 32x32 PNG tiles side-by-side
2. Place in `assets/tilesets/`
3. Update `Map.cpp` level data values if needed
4. Rebuild

### Adding Audio
1. Place WAV/OGG/MP3 file in `assets/music/`
2. Call `audioManager.PlayMusic("path/to/file.wav", -1)`
3. Rebuild

---

## Debugging Tips

### Compilation Errors
- Ensure all `#include` paths are correct
- Check that classes are declared in headers
- Verify CMakeLists.txt includes all source files

### Runtime Issues
- Check console output for error messages
- Verify asset file paths (relative to `build/` directory)
- Use `../assets/` prefix for loading from build directory

### Performance
- Frame rate shown in build: "FPS: XX" in top-left (if implemented)
- Use profiler in Visual Studio for detailed analysis
- Keep Update() functions lightweight

---

## Common Commands

| Command | Purpose |
|---------|---------|
| `.\build.bat` | Full rebuild |
| `cd build && .\MemeLifeSDL2.exe` | Run game |
| `python generate_assets.py` | Regenerate sprites/tileset |
| `rm -r build/*` | Clean build output |

---

## Troubleshooting

### "Cannot find tileset" error
- Check that `assets/tilesets/world_tileset.png` exists
- Verify relative path from build directory: `../assets/tilesets/`

### Game window doesn't open
- Ensure SDL2 is properly installed via vcpkg
- Check that Visual Studio environment is set up

### No sound playing
- Verify `assets/music/theme.wav` exists
- Check console for "Now playing:" message
- Ensure SDL_mixer initialized successfully

---

## Next Phase Tasks

For Phase 2 (NPCs, Dialogue, Items):
1. Create `NPC.h/cpp` with dialogue system
2. Create `Item.h/cpp` with pickup/drop logic
3. Create `Inventory.h/cpp` for item storage
4. Expand Map with room transitions
5. Add state machine for game scenes

---

**Happy Developing!** 🚀
