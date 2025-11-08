# Phase 2 — Completed Features

This document lists the Phase 2 work that has been implemented in the repository and gives a short description of each addition so reviewers and teammates can quickly verify functionality.

## Goal
Phase 2 focused on building a contained, playable Level 1 with movement, event quizzes, a boss with a finisher mechanic, and basic save/load support.

## What was added
- Renderer module
  - `include/Renderer.h`, `src/Renderer.cpp`
  - Centralized drawing helpers: `DrawBox`, `DrawText`, `DrawHUD`, `DrawMap`, `DrawPopup`, `DrawQuiz`, `DrawBattle`.
  - Ensures cursor-aware drawing so boxes and HUD align in the terminal.

- Map system
  - `include/Map.h`, `src/Map.cpp`
  - Static level layout with `#` boundaries, walkable tiles, event tiles `E`, and boss tile `B`.
  - `RandomizeEvents(n, keepBoss=false)` to scatter `E` events while keeping `B` fixed for the MVP.

- Quiz system
  - `include/Quiz.h`, `src/Quiz.cpp`
  - In-memory `QuizBank` with multiple quiz questions.
  - Word-wrapped quiz UI; correct answer increases a stat (Strength) and wrong answer reduces HP (with a visible popup).

- Battle system
  - `include/Battle.h`, `src/Battle.cpp`
  - Turn-based boss fight: normal attack, boss counterattack, and a finisher unlocked after a few player attacks.
  - Boss has higher HP than the player; boss attacks deal significant damage.
  - Win leads to level-up, lose shows a defeat message and returns to the menu.

- Save / Load
  - `include/Save.h`, `src/Save.cpp`
  - Save player stats, position, and full map grid to `memelife.sav` and load them later.

- Game state machine and wiring
  - `src/main.cpp` refactored to a small state machine: MENU → MAP → QUIZ → BATTLE → POST_LEVEL → EXIT.
  - Movement with `WASD` (uppercase accepted), event triggers on `E`, boss trigger on `B`.
  - HUD drawn below the map showing Name, Type, HP, Clout, Logic, Sanity, Strength, MemePower.

- UI & polish
  - Fixed mojibake in quiz strings by replacing smart quotes with ASCII equivalents.
  - Word-wrap for quizzes so long questions do not overflow the box.
  - Popups for correct/incorrect quiz answers that describe the stat changes and HP penalties.

## Controls (recap)
- WASD: Move (W = up, A = left, S = down, D = right)
- 1,2,...: Choose options in quizzes and in battle
- Q: Back / Quit in context

## How to build and run (Windows PowerShell)
1. Compile:

```powershell
g++ -std=c++11 -Iinclude src/main.cpp src/Terminal.cpp src/AssetLoader.cpp src/Renderer.cpp src/Map.cpp src/Quiz.cpp src/Battle.cpp src/Save.cpp -o MemeLife.exe
```

2. Run:

```powershell
.\MemeLife.exe
```

(There are also `Makefile` and `CMakeLists.txt` entries if you prefer those workflows.)

## Verification checklist
- [x] Map renders at top-left and HUD appears below map
- [x] WASD movement works with uppercase or lowercase input
- [x] Standing on `E` triggers quiz; correct answers give stat increases; wrong answers deal HP damage and show a message
- [x] Boss at `B` opens a battle screen; finisher unlocks after a few attacks; finisher ends the fight
- [x] Save and Load store and restore player and map state
- [x] Game compiles on Windows with g++ (C++11) and runs

## Known limitations / future work
- Keyboard arrows not supported (WASD only)
- Quiz bank is in-code (not yet loaded from external files). A file-based parser is a clear next step.
- UI uses text-only primitives; color and animations would improve feedback.
- Post-level flow (Level 2) is only a placeholder in the current build.

## Next recommended items
1. Move quiz bank to an external asset file for easier editing.
2. Add arrow-key movement & input debouncing.
3. Improve battle AI and scale damage with player stats.
4. Add unit tests for Save/Load and parsing of events.

---

**Phase 2 complete — ready for review and Phase 3 planning.**
