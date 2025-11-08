### **Master Plan: "MemeLife - The Hostel Years"**

### **1. High-Level Vision (The Elevator Pitch)**

"MemeLife" is a top-down, 2D Life Simulation RPG built with SDL2. It fuses the strategic stat management of a life-sim with the exploration and event-driven gameplay of classic Pokémon titles. Players will navigate the chaotic, meme-filled life of an Indian college student living in a hostel, starting at age 16. Every choice, every interaction, and every day survived contributes to their evolution from a clueless fresher to a legendary "Chad" or a broken NPC. The game is a satirical celebration of Indian youth culture, packed with meme sound effects, regional stereotypes, and absurd "canon events."

### **2. Core Design Pillars**

1.  **Top-Down RPG Exploration:** The game is a visual, interactive experience. The player controls a sprite on a tile-based map, encouraging exploration and discovery. This is a fundamental shift from the original text-based concept.
2.  **Satirical Life Simulation:** At its heart, this is a game about managing resources: **Time**, **Money**, and the core stats of **Clout**, **Logic**, and **Sanity**. The date/time system is central to progression.
3.  **Event-Driven Narrative:** The story unfolds through interactions with NPCs and objects on the map. Every item, from the hostel mess food to a professor's lecture notes, can trigger a unique event, dialogue, or stat check.
4.  **Meme-Infused Presentation:** The game's identity is its humor. This will be delivered through a constant barrage of meme sound effects, visual gags, witty dialogue, and satirical character archetypes.

### **3. Technical Foundation**

*   **Engine:** C++ with the **SDL2** ecosystem.
*   **Graphics:** **SDL2 (Core)** for rendering 2D sprites and tilemaps. **SDL2_image** for loading `.png` assets.
*   **Text:** **SDL2_ttf** for rendering all in-game text via a loaded font file.
*   **Audio:** **SDL2_mixer** is now a **critical library** for handling background music and the numerous `.wav` or `.ogg` meme sound effects.

### **4. The Player's Journey (Game Flow)**

1.  **Launch:** The game window opens with a splash screen and a greeting. A main menu theme plays.
2.  **Main Menu:** Clean and simple options: **New Game, Load Game, Settings, Exit**.
3.  **New Game Sequence:**
    *   The player selects "New Game."
    *   A loading screen appears ("Generating Your Canon Events..."). The "Birth Music" track plays.
    *   The **Character Creation** screen appears.
        *   The player chooses their birth state from a list of Indian states.
        *   The choice reveals a humorous stereotype-based **Buff** and **Debuff** (e.g., "Born in Punjab: **Buff:** *Hearty Eater* (more energy from food). **Debuff:** *Loud Music Addiction* (randomly loses money on new speakers).")
        *   The player confirms their character.
4.  **The Game Begins:** The player character spawns inside their hostel room. It's Day 1, Age 16. The core gameplay loop starts.
5.  **Core Gameplay Loop:**
    *   The player explores the world (Hostel, College Campus, City) in a top-down view.
    *   They interact with objects and NPCs by walking up to them and pressing a key.
    *   Interactions trigger events: dialogue boxes, stat checks, battles, or item acquisitions.
    *   The in-game clock progresses. Specific events may only be available at certain times of day.
    *   Progression is measured in levels, which are gained after a set number of in-game days have passed. Leveling up increases base stats and marks the passage of time (and age).
6.  **Endgame Scenarios:**
    *   **Success:** The player successfully reaches Level 10 (around Age 26), triggering a final "graduation" event based on their dominant stats.
    *   **Failure (Health):** The player is hospitalized 5 times. On the 5th visit, it's Game Over.
    *   **Failure (Canon Event):** The player makes a choice during a critical, scripted event that leads to an immediate, humorous Game Over.

### **5. Core Gameplay Systems (The Mechanics)**

*   **Character System:**
    *   **Primary Stats:** Clout, Logic, Sanity. These determine success in events.
    *   **Secondary Stats:** Energy (depletes over the day, restored by sleeping/eating), Money.
    *   **Buffs/Debuffs:** Permanent modifiers applied at character creation.

*   **Map & Exploration System:**
    *   The world will be built using a **tile-based map** system. We'll use a simple 2D array or a tool like "Tiled Map Editor" to design maps.
    *   The player will be a sprite that can move in 4 or 8 directions.
    *   **Collision Detection** will be implemented to prevent walking through walls.
    *   **Interaction Tiles** will be special tiles that, when the player is near and presses a button, trigger an event.

*   **Date/Time & Progression System:**
    *   A global clock will run continuously. An in-game day might last 15-20 real-world minutes.
    *   The screen can tint to represent morning, day, evening, and night.
    *   **Leveling is tied to time:** `Level Up = X days survived`.
    *   **Total Playable Levels:** 10 (from Age 16 to ~26).

*   **Health & Failure System:**
    *   A global integer, `hospitalizations`, starts at 0.
    *   Losing a battle or failing a critical health-related event increments this counter and sends the player back to their room (after losing time and money).
    *   If `hospitalizations` reaches 5, the game ends.
    *   "Canon Events" are hard-coded narrative events with a high risk of an instant Game Over.

*   **Asset Management:**
    *   **Visuals:** All characters, environments, and UI elements will be `.png` sprites.
    *   **Audio:** A dedicated `AudioManager` will handle two channels:
        1.  A **Music Channel** for continuous background tracks (Hostel theme, College theme, etc.).
        2.  Multiple **Sound Effect Channels** to allow for rapid, overlapping playback of meme sounds (`.wav` files).

### **6. Development Roadmap (The New Phased Plan)**

This is a much larger project. We must build it in layers.

*   **Phase 1: The World Engine.**
    *   **Goal:** Create a movable character on a static, tile-based map.
    *   **Tasks:** SDL2 setup, window creation, load a tileset and player sprite, implement player movement with collision, and play a single background music track. **NO GAMEPLAY.**

*   **Phase 2: The Interactive Layer.**
    *   **Goal:** Make the world feel alive and interactive.
    *   **Tasks:** Implement the Date/Time system. Create a robust **Dialogue Box/UI system** for displaying text. Implement the interaction system (walk to an object, press 'E', and see a dialogue box).

*   **Phase 3: The Core Game Loop & Systems.**
    *   **Goal:** Implement all the underlying simulation mechanics.
    *   **Tasks:** Build the Character Creation screen with the buff/debuff system. Implement the core stats (Clout, Logic, Sanity) and make events actually modify them. Code the Health/Hospitalization system and the time-based Leveling system.

*   **Phase 4: Content, Memes, and Polish.**
    *   **Goal:** Turn the engine into a complete game.
    *   **Tasks:** Design and build all the maps (Hostel, College, etc.). Write all the dialogue, events, and character interactions. Integrate the dozens of meme sound effects. Add the Main Menu, Save/Load functionality, and balance the game's difficulty.