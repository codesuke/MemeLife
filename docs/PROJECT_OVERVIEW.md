# MemeLife - Project Overview

## 🎯 What is MemeLife?

MemeLife is a terminal-based narrative game where players navigate through the world of meme culture, making choices that affect their character's stats (Clout, Logic, Sanity, HP). The game is built with a data-driven architecture, allowing easy expansion through text files without recompiling code.

---

## 🏗️ Architecture

### Design Philosophy
- **Data-Driven**: Game content loaded from external text files
- **Modular**: Separated concerns (Terminal, Assets, Game Logic)
- **Cross-Platform**: Works on Windows, Linux, and macOS
- **Expandable**: Easy to add new events, choices, and content

### Core Components

#### 1. Terminal Control (`Terminal.h/cpp`)
Provides low-level terminal manipulation:
- Screen clearing
- Cursor positioning
- Color control
- Immediate input (no Enter key needed)

#### 2. Data Structures (`GameData.h`)
Defines the game's data models:
- **Stats**: Character statistics (Clout, Logic, Sanity, HP)
- **Player**: Player state (Name, Type, Level, XP, Stats)
- **Choice**: Decision options with outcomes
- **GameEvent**: Narrative moments with descriptions and choices

#### 3. Asset Loader (`AssetLoader.h/cpp`)
Parses event files:
- Reads `.txt` files from `assets/events/`
- Parses prefixed lines (ID, DESC, CHOICE, ART)
- Returns structured GameEvent objects

#### 4. Game Engine (`main.cpp`)
The main game loop:
- Loads events
- Renders content
- Handles player input
- Manages game state

---

## 📂 Directory Structure Explained

```
MemeLife/
│
├── src/                    # Implementation files
│   ├── main.cpp           # Entry point, Game class, main loop
│   ├── Terminal.cpp       # Terminal control implementation
│   └── AssetLoader.cpp    # Event file parsing logic
│
├── include/               # Header files (interfaces)
│   ├── GameData.h        # Data structure definitions
│   ├── Terminal.h        # Terminal function declarations
│   └── AssetLoader.h     # Asset loading function declarations
│
├── assets/               # Game content (non-code)
│   └── events/          # Event text files
│       └── *.txt        # Individual event files
│
├── docs/                # Project documentation
│   ├── PHASE1_COMPLETE.md
│   └── verify-phase1.ps1
│
├── Instructions/        # Development phase guides
│   ├── Master.md       # Overall project roadmap
│   └── Phase*.md       # Phase-specific instructions
│
├── .vscode/            # VS Code IDE configuration
│   └── settings.json   # C++ IntelliSense & build settings
│
├── CMakeLists.txt      # CMake build script
├── Makefile           # Make build script
├── build_and_run.bat  # Windows quick build script
├── .gitignore         # Git ignore patterns
└── README.md          # Main project documentation
```

---

## 🔄 Game Flow

```
[Start] → Load Event → Display Event
           ↓
    Show Choices → Wait for Input
           ↓
    Process Choice → Load Next Event
           ↓
    [Loop continues...]
```

### Event File Format

```
ID: unique_event_name
DESC: The event description that the player sees.
CHOICE: Choice text shown to player | next_event_id
CHOICE: Another choice text | another_event_id
ART: Optional ASCII art line
ART: Another ASCII art line
```

### Example Event Flow

```
01_start.txt
├─ Choice 1 → 02_first_choice.txt
└─ Choice 2 → 03_logic_choice.txt
                ├─ Choice A → 04_outcome_a.txt
                └─ Choice B → 04_outcome_b.txt
```

---

## 🛠️ Build Systems

### Why Multiple Build Systems?

1. **Makefile**: Simple, fast, widely supported
2. **CMake**: Cross-platform, IDE integration, scalable
3. **build_and_run.bat**: Quick Windows testing

### Compilation Process

```
Source Files (.cpp) 
    ↓
Preprocessing (include headers)
    ↓
Compilation (create .o files)
    ↓
Linking (combine into executable)
    ↓
MemeLife.exe
```

---

## 📊 Data Flow

```
Event File (01_start.txt)
    ↓
AssetLoader::LoadEvent()
    ↓
GameEvent struct
    ↓
Game::currentEvent
    ↓
Terminal::ClearScreen() + Display
    ↓
Terminal::GetImmediateInput()
    ↓
Process Choice
    ↓
[Load next event...]
```

---

## 🎮 Player State Management

```cpp
Player {
    name: "PlayerName"
    memeType: "Troll"
    level: 1
    xp: 0
    stats: {
        clout: 50
        logic: 50
        sanity: 50
        hp: 100
        maxHp: 100
    }
}
```

Stats will be modified by choices in future phases.

---

## 🚀 Development Phases

### Phase 1: Core Engine ✅
- Project structure
- Terminal control
- Data structures
- Game loop
- Asset loading

### Phase 2: Enhanced Gameplay 🔜
- Choice selection
- Event navigation
- Stat modifications
- Save/Load system

### Phase 3: Advanced Mechanics 📋
- Combat system
- Inventory
- Advanced ASCII art
- Sound effects

### Phase 4: Polish & Features 📋
- Main menu
- Settings
- Multiple endings
- Achievement system

---

## 🧪 Testing Strategy

1. **Build Test**: Verify compilation succeeds
2. **Run Test**: Ensure executable launches
3. **Event Test**: Confirm event loading works
4. **Input Test**: Validate keyboard input
5. **Display Test**: Check terminal rendering

Use `docs/verify-phase1.ps1` to automate Phase 1 testing.

---

## 🔧 Common Development Tasks

### Adding a New Event
1. Create `assets/events/XX_event_name.txt`
2. Follow the event format (ID, DESC, CHOICE)
3. Link from existing events via CHOICE outcomes
4. Test by navigating to it in-game

### Adding a New Module
1. Create `include/ModuleName.h` (declarations)
2. Create `src/ModuleName.cpp` (implementation)
3. Update `CMakeLists.txt` and `Makefile`
4. Include in `main.cpp` where needed

### Debugging
1. Add print statements: `cout << "DEBUG: " << variable << endl;`
2. Use VS Code debugger (F5)
3. Check that event files are in correct location
4. Verify file paths in LoadEvent()

---

## 📖 Code Style Guide

- **Indentation**: 4 spaces
- **Naming**: 
  - Classes/Structs: `PascalCase`
  - Functions: `PascalCase`
  - Variables: `camelCase`
  - Constants: `UPPER_SNAKE_CASE`
- **Comments**: Explain WHY, not WHAT
- **Files**: One class per file pair (.h/.cpp)

---

## 🤝 Contributing

This is a college project, but if working in a team:
1. Create feature branches
2. Test before committing
3. Write clear commit messages
4. Update documentation
5. Follow the existing code style

---

## 📚 Resources

- **C++ Reference**: https://cppreference.com/
- **ANSI Escape Codes**: https://en.wikipedia.org/wiki/ANSI_escape_code
- **CMake Documentation**: https://cmake.org/documentation/
- **Game Design**: See `Instructions/Master.md`

---

**Last Updated**: Phase 1 Complete
