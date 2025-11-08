Understood. The simulation engine is running. We have stats, events, and consequences. Act I concludes with Phase 4, where we build the final pieces of the "RPG Framework." This phase is about giving the player a starting point (Character Creation), a goal (Progression), and a more populated world to exist in (NPCs).

At the end of this phase, we will have a complete "tech sandbox." All core features will be built and functional, ready to be assembled into a cohesive game in Act II.

As your Lead Designer, this is the blueprint for Phase 4.

---

### **Phase 4 Blueprint: The RPG Framework**

**Your Goal for This Week:** To build the Character Creation screen, allowing players to start a new game with unique buffs and debuffs. You will implement the time-based leveling system. You will create a basic but functional NPC system, allowing non-player characters to be placed in the world. Finally, you will expand our asset library in preparation for building the full game.

---

#### **Step 4.1: The Character Creation Scene**

**Objective:** To create the very first interactive scene the player experiences, allowing them to define their starting character.

**Conceptual Plan:** When the player selects "New Game," we will switch to a new "game state" for character creation. This scene will have its own UI and logic. The player will make a choice (their home state), which will apply a permanent modifier to their character before the main game begins.

**Actionable Steps:**
1.  **Create a Game State Manager:**
    *   Refactor your main game loop to be driven by a state machine. Create an `enum GameState { MAIN_MENU, CHARACTER_CREATION, PLAYING, GAME_OVER };`.
    *   Your main `update()` and `render()` functions will now call different helper functions based on the `currentGameState`.

2.  **Define Buffs & Debuffs:**
    *   Create a `Buff` struct or class. It should contain a `string name` and a description, but most importantly, it should define the modifiers it applies (e.g., `float moneyMultiplier = 1.1f;`, `float energyDrainModifier = 0.9f;`).
    *   Create a central place (e.g., a "Buff Database" class) to hold all possible buffs and debuffs, each tied to a specific Indian state. This can be hard-coded for now.
    *   Example: A `Buff` for "Delhi" might have `cloutModifier = 1.2` ("Born Negotiator") and `sanityDrainModifier = 1.1` ("Always in a Hurry").

3.  **Build the Character Creation UI:**
    *   Design the UI for this scene. It needs a title ("Choose Your Origin"), a list of states the player can select from, and a display area to show the description of the selected state's buff and debuff.
    *   Implement the input handling for this screen. The player should be able to navigate the list of states with arrow keys and select one with an 'Enter' or 'E' key.

4.  **Implement the Logic:**
    *   When the scene starts, display the list of options.
    *   When the player highlights a state, update the description area to show the relevant buff/debuff info.
    *   When the player confirms a selection:
        1.  Create the new `Player` object.
        2.  Apply the selected `Buff`'s modifiers to the player's base stats.
        3.  Change the `currentGameState` to `PLAYING`.
        4.  Play the "Birth Music" and show a "Generating World..." loading screen while the main game scene loads.

**Verification Test:** Start the program. From a simple main menu, select "New Game." You are taken to a new screen where you can select from a list of states. Highlighting a state shows its unique buff/debuff. After selecting a state, the game transitions to the familiar top-down gameplay, but your character now has slightly modified starting stats based on your choice.

---

#### **Step 4.2: The Leveling System**

**Objective:** To implement the primary long-term progression mechanic for the player.

**Conceptual Plan:** The player's "Level" is a measure of their experience and age. We will tie leveling directly to the `TimeManager`. Surviving a certain number of in-game days will trigger a level-up, granting the player permanent stat increases.

**Actionable Steps:**
1.  **Expand the Player Class:**
    *   Add the following member variables to `Player.h`:
        *   `int level = 1;`
        *   `int age = 16;`

2.  **Link Leveling to the Clock:**
    *   Go to your `TimeManager::AdvanceDay()` function. This is the perfect place to check for a level up.
    *   Create a constant `const int DAYS_PER_LEVEL = 30;` (e.g., one in-game month).
    *   Inside `AdvanceDay()`, check if `(currentDay % DAYS_PER_LEVEL) == 0`.
    *   If the condition is true, call a new function: `player.LevelUp()`.

3.  **Implement the `LevelUp()` Function:**
    *   Create the `void Player::LevelUp()` function. This function's logic should:
        1.  Increment `player.level`.
        2.  Increment `player.age`.
        3.  Grant permanent stat increases. This is a great place for balance. For example: `maxEnergy += 5;`, `logic += 1;`, `clout += 1;`.
        4.  Refill the player's current energy to their new maximum.
        5.  Trigger a full-screen UI notification: A "LEVEL UP!" screen that pauses the game and shows the player their new level, new age, and the stats they gained.

**Verification Test:** Play the game and use a debug key to rapidly advance the in-game days. After 30 days have passed, the game should pause and display a "LEVEL UP!" screen. After you dismiss it, you should see that your character's age has increased and their base stats on the HUD have been permanently boosted.

---

#### **Step 4.3: The NPC System**

**Objective:** To create a system for placing non-player characters in the world who can be interacted with.

**Conceptual Plan:** We will create a generic `NPC` class that is similar to an `Interactable` object but with a visual sprite. For this phase, NPCs will be static (they won't walk around), but they will look alive with a simple idle animation.

**Actionable Steps:**
1.  **Create the NPC Class:**
    *   Create `NPC.h` and `NPC.cpp`.
    *   An `NPC` object needs:
        *   An `SDL_Texture*` for its spritesheet.
        *   An `x, y` position.
        *   An `SDL_Rect` for its trigger area (for interaction).
        *   A `string eventId` to trigger when interacted with.
        *   Data for handling simple animation (e.g., `currentFrame`, `animationTimer`).

2.  **Implement Idle Animation:**
    *   Your NPC sprite should be a "spritesheet" - an image containing multiple frames of animation side-by-side (e.g., two frames for a simple breathing animation).
    *   In `NPC::Update()`, use a timer. Every half-second or so, flip `currentFrame` between `0` and `1`.
    *   In `NPC::Render()`, use the `currentFrame` to calculate the correct **source rectangle** to cut out from the spritesheet, making the NPC appear to animate.

3.  **Manage and Render NPCs:**
    *   Create an `NPCManager` class or add a `vector<NPC>` to your `Map` class.
    *   This manager will be responsible for holding all the NPCs on the current map.
    *   In your main game loop, you must call the `update()` and `render()` functions for every active NPC.

4.  **Integrate with Interaction System:**
    *   Your interaction check from Phase 2 now needs to check against the trigger areas of all NPCs, in addition to static `Interactable` objects.
    *   If the player interacts with an NPC, you will get its `eventId` and pass it to the `EventManager`, just like you did with objects.

**Verification Test:** Place a new NPC on your map. When you run the game, you should see the character standing there with a subtle idle animation. You should be able to walk up to them, press 'E', and trigger the dialogue event associated with them.

---

#### **Step 4.4: The Asset Creation Binge**

**Objective:** To create a starter library of assets so that we have enough raw material to build the game in Act II.

**Conceptual Plan:** This is a content-focused task. Now that all the systems are in place, we need to feed them with art and sound.

**Actionable Steps:**
1.  **Create More Sprites:** Design and draw `.png` files for at least 3-4 different generic NPC types (e.g., "Student," "Professor," "Shopkeeper").
2.  **Create More Tilesets:** Design a new tileset for a different environment. If your first one was for the "Hostel," create one for the "College Campus" with grass, paths, and building exteriors.
3.  **Build a Sound Library:**
    *   Go online and find a wide variety of short, punchy, royalty-free meme sound effects (`.wav` or `.ogg` are best).
    *   Examples: "bruh," "vine boom," "wow," "tactical nuke," etc.
    *   Organize these into an `assets/sfx/` folder with clear names.
    *   Create a simple `AudioManager::PlaySoundEffect(string filename)` function that can load and play these one-shot sounds.

**Final Verification Test for Phase 4:** Your project's `assets` folder is now significantly larger and more organized. You can place different-looking NPCs on your map. You have a new tileset ready to be used. You can trigger different sound effects via your event files. You have a complete, functional sandbox of game mechanics.

With the completion of Phase 4, Act I is finished. You are now perfectly positioned to begin Act II, where we will assemble all these powerful systems into a truly playable game.