### **Phase 1: The Foundation - Core Engine & Systems (Week 1)**

**Goal:** To build a non-interactive but functional program that can read game data and manage the terminal window. This is the bedrock of our project.

*   **Step 1.1: Environment and Project Setup**
    *   **Action:** Choose and configure your compiler (GCC/G++ on Linux/Mac, MinGW or MSVC on Windows).
    *   **Action:** Create the project directory:
        ```
        MemeLife/
        ├── src/      (for .cpp files)
        ├── include/  (for .h files)
        ├── assets/   (for all game data)
        └── Makefile or CMakeLists.txt
        ```
    *   **Action:** Create your `main.cpp` file.

*   **Step 1.2: Terminal Control Module**
    *   **Action:** Create `Terminal.h` and `Terminal.cpp`.
    *   **Action:** Implement `clearScreen()` using platform-specific preprocessor directives (`#ifdef _WIN32` for `system("cls")` and `#else` for ANSI code `"\033[2J"`).
    *   **Action:** Implement `moveCursor(int x, int y)` and `setColor(color)` using standard ANSI escape codes. These are portable.
    *   **Action:** Implement a portable `getRawInput()` function. This is a key challenge. Use `<conio.h>` and `_getch()` for Windows, and use `<termios.h>` to temporarily disable canonical mode and echo on Linux/macOS.
    *   **Milestone:** A program that clears the screen, prints "TEST" in red, waits for a single keypress without needing Enter, and then exits.

*   **Step 1.3: Core Data Structures**
    *   **Action:** Create a single header, `GameData.h`, to hold all primary structs.
    *   **Action:** Define `struct Stats { int clout, logic, sanity, hp, maxHp; }`.
    *   **Action:** Define `struct Player { string name, memeType; int level, xp; Stats stats; }`.
    *   **Action:** Define `struct Choice { string text; string outcomeType; // e.g., "EVENT", "BATTLE", "STAT_CHANGE" string outcomeValue; }`.
    *   **Action:** Define `struct GameEvent { string id, description; vector<string> asciiArt; vector<Choice> choices; }`.

*   **Step 1.4: Main Game Loop and State Manager**
    *   **Action:** In `main.cpp`, create a central `Game` class.
    *   **Action:** The `Game` class will hold the `Player` object and a `string currentEventId`.
    *   **Action:** Implement the main loop: `while (isRunning) { handleInput(); update(); render(); }`. Stub these functions for now.
    *   **Milestone:** A program that initializes a player with default stats and loops infinitely, printing the player's HP to the console on each loop.

*   **Step 1.5: The Asset Parser**
    *   **Action:** Create `AssetLoader.h` and `AssetLoader.cpp`.
    *   **Action:** Implement `GameEvent loadEventFromFile(const string& eventId)`. This function will open `assets/events/` + `eventId` + `.txt`.
    *   **Action:** The parser will read the file line-by-line using prefixes. Example format in `01_start.txt`:
        ```
        ID: 01_start
        DESC: You wake up. The terminal beckons.
        CHOICE: Get up. | EVENT | 02_first_choice
        ```
    *   **Milestone:** The game starts, loads `01_start.txt` into a `GameEvent` object, prints "You wake up. The terminal beckons." and then exits.

### **Phase 2: Bringing the World to Life - UI & Interactivity (Week 2)**

**Goal:** To make the game look and feel interactive. The player can now make choices and see the consequences.

*   **Step 2.1: The UI & HUD Renderer**
    *   **Action:** Create `Renderer.h` and `Renderer.cpp`.
    *   **Action:** Implement `drawBox(x1, y1, x2, y2)` to draw frames using ASCII characters.
    *   **Action:** Implement `drawText(x, y, text)`.
    *   **Action:** Implement `drawHUD(const Player& player)`. This function will draw a main border and the player's stats (including an HP bar like `[#######---]`) in a fixed position.
    *   **Milestone:** The game screen now has a persistent, professional-looking border and HUD that displays the player's stats.

*   **Step 2.2: Event System Integration**
    *   **Action:** Update the `render()` function to call the `Renderer` to draw the current event's description and choices within the main window.
    *   **Action:** Update `handleInput()` to capture number keys ('1', '2', etc.).
    *   **Action:** In the `update()` function, if a choice was made, process its outcome. This means updating the `Game` object's `currentEventId` based on the chosen `Choice`.
    *   **Milestone:** A fully interactive "choose your own adventure." The player can navigate through a chain of 3-4 connected events loaded from text files.

*   **Step 2.3: ASCII Sprite Handling**
    *   **Action:** In `AssetLoader`, add a function to load a multi-line ASCII art file into a `vector<string>`.
    *   **Action:** In `Renderer`, add a `drawAsciiArt(x, y, const vector<string>& art)` function.
    *   **Action:** Update the event format and parser to include an art tag: `ART: player_idle`.
    *   **Milestone:** Events can now display ASCII art, making the world feel more populated.

*   **Step 2.4: Save & Load System**
    *   **Action:** Create `saveGame(const Game& game)` and `loadGame(Game& game)` functions.
    *   **Action:** `saveGame` will write key variables (player stats, `currentEventId`) to `memelife.sav` in a simple text format.
    *   **Action:** `loadGame` will read this file and populate the `Game` object. Include error handling for when the file doesn't exist.
    *   **Milestone:** The game presents a main menu on startup: "1. New Game", "2. Continue". Progress is now persistent.

### **Phase 3: The Heat of Battle - Combat System (Week 3)**

**Goal:** To implement the game's core combat loop, including moves, UI, and visual feedback.

*   **Step 3.1: Battle Framework and Data**
    *   **Action:** In `GameData.h`, add `struct Enemy` and `struct BattleMove`.
    *   **Action:** Create a `BattleManager` class. It will manage the player, the current enemy, and an `enum BattleState { PLAYER_TURN, ENEMY_TURN, ... }`.
    *   **Action:** Update the event parser to recognize a "BATTLE" outcome, which will trigger the start of a battle.
    *   **Milestone:** An event choice can now transition the game state from "exploring" to "in battle," loading the appropriate enemy data.

*   **Step 3.2: Battle UI and Rendering**
    *   **Action:** Create a new `Renderer::drawBattleScreen()` function.
    *   **Action:** This function will display the player and enemy ASCII art, their respective HP bars, a menu for the player's available moves, and a text box for the battle commentary log.

*   **Step 3.3: Turn-Based Logic Implementation**
    *   **Action:** Code the state machine in `BattleManager::update()`.
    *   **Action:** For `PLAYER_TURN`: The `handleInput()` function now moves a cursor over the move list. On selection, calculate damage (with RNG), apply it to the enemy, add a line to the commentary log, and switch the state to `ENEMY_TURN`.
    *   **Action:** For `ENEMY_TURN`: Implement simple AI (e.g., choose a random move), apply damage to the player, update the log, and switch back to `PLAYER_TURN`.
    *   **Action:** After every action, check if player or enemy HP is <= 0 to trigger the `WIN` or `LOSE` states.
    *   **Milestone:** A complete, interactive battle loop. The player can fight and defeat an enemy (or be defeated).

*   **Step 3.4: Basic ASCII Animations**
    *   **Action:** Create an `Animation` asset loader that reads multiple frames from a file, separated by a delimiter.
    *   **Action:** Create an `AnimationPlayer` with a `play(animation)` function. This function will loop through the frames, calling `moveCursor`, `drawAsciiArt`, and `this_thread::sleep_for()` to create the illusion of movement.
    *   **Action:** In `BattleManager`, when a move is used, call the `AnimationPlayer` to play a simple effect.
    *   **Milestone:** Attacks are now accompanied by a simple visual effect, making combat much more engaging.

### **Phase 4: Content, Polish & Replayability (Week 4)**

**Goal:** To turn the tech demo into a finished, fun, and replayable game.

*   **Step 4.1: Content Creation Sprint**
    *   **Action:** Write the main storyline with branching paths using at least 20-30 event files.
    *   **Action:** Create 5-7 unique enemies (Karen, Sigma Male, NPC Horde, etc.) with distinct stats, ASCII art, and move sets.
    *   **Action:** Create ASCII art for the player's evolution stages.

*   **Step 4.2: Progression, Evolution, and Endings**
    *   **Action:** Implement the XP system. Award XP after winning a battle.
    *   **Action:** Create an `addXp()` function in the `Player` class. When XP crosses a threshold, trigger a `levelUp()` function that increases stats, changes the `memeType` string, and potentially adds new moves to the player's move list.
    *   **Action:** Design the final boss event. After this event, a special function will check the player's highest/lowest stats and load the appropriate ending event file.
    *   **Milestone:** A complete gameplay arc. The player can start weak, grow powerful, and achieve a final ending.

*   **Step 4.3: Polishing and Final Features**
    *   **Action:** **Achievements:** Add a `map<string, bool>` to the `Game` object. Implement checks for unlocking achievements (e.g., "Win a battle with 1 HP left"). Add a viewer in the main menu.
    *   **Action:** **Easter Eggs:** Add a hidden event or a cheat code that can be entered.
    *   **Action:** **Balancing:** Playtest the game extensively. Adjust enemy HP, damage values, and stat checks to ensure the difficulty is challenging but fair.
    *   **Action:** **Code Cleanup:** Review the code for clarity, add comments, and fix any remaining bugs.

*   **Step 4.4: Final Build and Distribution**
    *   **Action:** Compile final executables for Windows, macOS, and Linux.
    *   **Action:** Write a clear `README.md` file with instructions.
    *   **Action:** Package the executable and the `assets` folder into a `.zip` file.
    *   **Milestone:** **Project "MemeLife: Rise of the NPC" is complete and ready to be shared!** **Project "MemeLife: Rise of the NPC" is complete and ready to be shared!**