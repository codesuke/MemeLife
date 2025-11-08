# Contributing to MemeLife

Thanks for wanting to contribute! This document explains our development process and guidelines.

## Before You Start

1. **Read README.md** - understand the project structure
2. **Set up your environment** - run `setup.ps1` to get everything installed
3. **Build and run** - make sure `build.bat` and the game work
4. **Read through the code** - get familiar with existing components

## Development Process

### 1. Choose/Create an Issue
- Check existing issues to avoid duplicates
- Describe what you want to add/fix
- Ask maintainers if unsure about approach

### 2. Create a Feature Branch
```bash
git checkout -b feature/description-of-feature
```

**Naming conventions:**
- `feature/add-npc-system` - new feature
- `bugfix/fix-dialogue-crash` - bug fix
- `docs/update-readme` - documentation
- `refactor/optimize-rendering` - refactoring

### 3. Make Your Changes

#### Code Changes
- **One feature per branch** - don't mix multiple features
- **Test frequently** - build and run after each logical change
- **Write clear code** - comments for complex logic
- **Follow existing style** - camelCase for variables, PascalCase for classes

#### File Organization
```
src/           - Keep C++ files here
├── NewFeature.h       - Header file
└── NewFeature.cpp     - Implementation
assets/        - Keep game content here
├── sprites/    - PNG images
├── tilesets/   - PNG tilesets
├── music/      - WAV/OGG audio
├── fonts/      - TTF fonts
├── sfx/        - Sound effects
└── events/     - Event definition files
```

#### Adding a New Class

**Example: Creating a ParticleSystem**

1. **Create header** (`src/ParticleSystem.h`):
```cpp
#pragma once
#include <SDL2/SDL.h>
#include <vector>

class ParticleSystem {
public:
    ParticleSystem(SDL_Renderer* renderer);
    ~ParticleSystem();
    
    void Update(float deltaTime);
    void Render();
    
    void Emit(int x, int y);
    
private:
    struct Particle {
        float x, y;
        float vx, vy;
        float lifetime;
    };
    
    SDL_Renderer* renderer;
    std::vector<Particle> particles;
};
```

2. **Create implementation** (`src/ParticleSystem.cpp`):
```cpp
#include "ParticleSystem.h"
#include <iostream>

ParticleSystem::ParticleSystem(SDL_Renderer* renderer)
    : renderer(renderer) {
}

// ... implement methods
```

3. **Add to CMakeLists.txt**:
```cmake
add_executable(${PROJECT_NAME}
    # ... existing files
    src/ParticleSystem.cpp
    src/ParticleSystem.h
)
```

4. **Use in main.cpp**:
```cpp
#include "ParticleSystem.h"

// In main():
ParticleSystem particles(renderer);

// In game loop update:
particles.Update(FRAME_TIME_SECONDS);

// In render section:
particles.Render();
```

### 4. Build and Test

```bash
# Full rebuild
build.bat

# Run the game
build\MemeLifeSDL2.exe

# Test your feature thoroughly
```

**Things to test:**
- ✓ Game starts without crashing
- ✓ Your feature works as expected
- ✓ Existing features still work
- ✓ No memory leaks (look for growing memory usage)
- ✓ Performance is acceptable (60 FPS)

### 5. Commit Your Changes

```bash
# Stage all changes
git add .

# Commit with clear message
git commit -m "Add particle system for visual effects

- Particles emit from specified position
- Each particle has lifetime and velocity
- Rendered as small colored squares
- Integrated with main game loop"
```

**Commit message style:**
- First line: ~50 characters, describes what changed
- Blank line
- Detailed explanation of why (not how)
- Reference issues if applicable: "Fixes #123"

### 6. Push and Create Pull Request

```bash
git push origin feature/your-feature-name
```

Then on GitHub:
1. Click "Compare & pull request"
2. Fill in PR title and description
3. Submit for review

**PR Description Template:**
```markdown
## What's This About?
Brief description of the feature/fix

## Changes Made
- Change 1
- Change 2
- Change 3

## How to Test
Steps to verify the feature works

## Screenshots/Video (if applicable)
Before/after comparisons

## Checklist
- [ ] Code builds without errors
- [ ] Game runs without crashes
- [ ] Feature tested thoroughly
- [ ] Commit messages are clear
- [ ] No unrelated changes included
```

## Code Guidelines

### Header Files
- Always use `#pragma once` guards
- Forward declare when possible
- Keep implementation in .cpp files

### Comments
```cpp
// ✓ Good: Explains WHY
if (player.GetEnergy() < 10.0f) {
    // Player is exhausted, can't perform intensive actions
    return false;
}

// ✗ Bad: Explains WHAT (code already shows this)
// Check if energy is less than 10
if (player.GetEnergy() < 10.0f) {
    return false;
}
```

### Memory Management
```cpp
// ✓ Good: Use smart pointers
std::unique_ptr<Texture> myTexture;
std::unique_ptr<Player> player;

// ✓ Acceptable: Raw pointers for references (no ownership)
void Update(Player* player) { }

// ✗ Avoid: Manual new/delete
Player* player = new Player();  // Memory leak risk!
```

### Naming
```cpp
// ✓ Good
class EnergyBar { }
float currentEnergy = 100.0f;
void UpdatePlayerStats() { }
const int MAX_HOSPITALIZATIONS = 5;

// ✗ Bad
class energybar { }           // Classes should be PascalCase
float ce = 100.0f;            // Variables should be descriptive
void updateplayerstats() { }   // Methods should be camelCase
const int max_hosp = 5;       // Constants should be UPPER_CASE
```

### Error Handling
```cpp
// ✓ Good: Handle errors gracefully
TTF_Font* font = TTF_OpenFont(fontPath.c_str(), 16);
if (!font) {
    std::cerr << "Failed to load font: " << fontPath << std::endl;
    std::cerr << "TTF_Error: " << TTF_GetError() << std::endl;
    return false;
}

// ✗ Bad: Crash silently
TTF_Font* font = TTF_OpenFont(fontPath.c_str(), 16);
// No check - crashes when dereferencing null pointer!
```

## Testing Checklist

Before submitting a PR, ensure:

- [ ] **Builds Successfully**
  ```bash
  build.bat
  ```
  No errors (warnings are OK if pre-existing)

- [ ] **Game Runs**
  ```bash
  build\MemeLifeSDL2.exe
  ```
  Starts without crashes

- [ ] **Feature Works**
  - Test all new functionality
  - Test edge cases
  - Test with different inputs

- [ ] **No Regressions**
  - Run through existing features
  - Character creation still works
  - Game over mechanic still works
  - All existing controls responsive

- [ ] **Performance**
  - FPS stays at ~60 (check output)
  - No stuttering or lag
  - No memory leaks (task manager)

## Code Review

When your PR is reviewed:

1. **Reviewers will check:**
   - Code quality and style
   - Functionality and correctness
   - Performance impact
   - Documentation/comments

2. **You might get feedback:**
   - "Can you refactor this section?"
   - "This needs better error handling"
   - "Performance concern here"

3. **Respond to feedback:**
   ```bash
   # Make requested changes
   # ...
   
   # Stage and commit
   git add .
   git commit -m "Address code review feedback"
   
   # Push again (same branch)
   git push origin feature/your-feature-name
   ```

4. **Once approved, we'll merge!**

## Common Issues

### "My changes are conflicting with main"
```bash
git fetch origin
git rebase origin/main
# Resolve conflicts in your editor
git add .
git rebase --continue
git push origin feature/name --force-with-lease
```

### "I committed to main by accident"
```bash
# Create new branch from current state
git branch feature/oops-main

# Reset main to before bad commit
git checkout main
git reset --hard origin/main

# Switch to feature branch
git checkout feature/oops-main
```

### "Build fails after my changes"
```bash
# Clean rebuild
rmdir /s build
build.bat

# If still fails, check:
# 1. Did you add new files to CMakeLists.txt?
# 2. Are all includes correct?
# 3. Are there typos in #include paths?
```

## Questions?

- **Project architecture**: See README.md
- **How to add a feature**: See "Adding a New Class" above
- **CMake stuff**: Check existing CMakeLists.txt
- **Git help**: Google "git how to..."
- **Still stuck**: Ask maintainers in an issue

---

**Thanks for contributing! 🚀**
