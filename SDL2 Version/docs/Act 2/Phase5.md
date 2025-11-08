Of course. Act I is complete. We have a sandbox full of powerful, independent systems. Now, we begin Act II, where we assemble those systems into a cohesive, playable game.

Phase 5 is the crucial first step of this act: transforming our single test room into a sprawling, interconnected world. This is where we stop being programmers and start being level designers.

As your Lead Designer, this is the blueprint for Phase 5.

---

### **Phase 5 Blueprint: Building the World**

**Your Goal for This Week:** To completely replace your hard-coded test map with a multi-map world designed in a professional-grade tool. You will create the main locations for the game (Hostel, College, City). You will implement a system for transitioning between these maps. Finally, you will populate these new maps with the interactive objects and NPCs you've already built.

---

#### **Step 5.1: Leveling Up Your Tools (Tiled Map Editor Integration)**

**Objective:** To stop defining maps in C++ code and start designing them visually in an external map editor.

**Conceptual Plan:** Hard-coding maps in a 2D array is slow, inefficient, and impossible for complex layouts. We will use a free, industry-standard tool called **Tiled Map Editor**. You will design your maps in Tiled, which exports them as a data file (like `.json`). We will then write a C++ parser to read this file and construct our map in-game.

**Actionable Steps:**
1.  **Download and Learn Tiled:**
    *   Download and install Tiled Map Editor. It's free.
    *   Watch a short "Beginner's Guide to Tiled" on YouTube. You need to understand three core concepts: **Tilesets**, **Tile Layers**, and **Object Layers**.

2.  **Create a Test Map in Tiled:**
    *   Create a new map in Tiled. Import your `world_tileset.png` as your tileset.
    *   Create **three layers**:
        *   A **Tile Layer** named `Background`: Paint all your floor tiles here.
        *   A **Tile Layer** named `Walls`: Paint all your solid, collidable wall tiles here.
        *   An **Object Layer** named `Objects`: This is where you will place triggers and entities later.
    *   Design a simple room and **export it as a JSON file** (e.g., `test_map.json`) into your `assets/maps/` folder.

3.  **Write the JSON Map Parser:**
    *   This is the main coding task. You will need a C++ JSON parsing library. **nlohmann/json** is highly recommended and easy to use.
    *   Create a new function in your `Map` class, `void LoadMapFromFile(string filename)`.
    *   The logic of this parser will be:
        1.  Open and parse the JSON file.
        2.  Read the map's `width` and `height` from the JSON.
        3.  Loop through the `layers` array in the JSON file.
        4.  If a layer is a "tilelayer" (like `Background` or `Walls`), read its `data` array into your map's internal 2D grid for rendering and collision.
        5.  If a layer is an "objectgroup" (like `Objects`), we will use this in Step 5.4.

4.  **Integrate the Parser:**
    *   Modify your game to call `map.LoadMapFromFile("assets/maps/test_map.json")` at the start instead of using the old hard-coded array.

**Verification Test:** When you run the game, it should now display the map you designed visually in Tiled. Your collision system should still work correctly with the walls you painted on the `Walls` layer.

---

#### **Step 5.2: Creating the World (Map Design & Creation)**

**Objective:** To use your new Tiled workflow to design and build the primary locations of your game.

**Conceptual Plan:** This is a creative, not a coding, step. Put on your level designer hat. Think about how the spaces should feel and how they connect. You will create the actual game world file by file.

**Actionable Steps:**
1.  **Plan Your World Layout:** On paper, sketch a simple diagram of how your main maps connect. (e.g., Hostel Room -> Hostel Hallway -> Campus Entrance -> City Street).
2.  **Build the Hostel Map:** In Tiled, create `hostel.json`. Design the player's room, a common area, maybe a friend's room, and the hallway connecting them.
3.  **Build the College Campus Map:** Create `campus.json`. This should be a larger, more open map. Design pathways, patches of grass, and the exteriors of several buildings (Library, Classroom Block, etc.).
4.  **Build the City Map:** Create `city.json`. A smaller map representing a street outside the campus, with a couple of storefronts.
5.  **Maintain Consistency:** Ensure you use the **exact same layer names** (`Background`, `Walls`, `Objects`) across all your map files.

**Verification Test:** You now have at least three distinct `.json` map files in your `assets/maps/` folder. You can temporarily change the filename in your `LoadMapFromFile` call to view each one in-game.

---

#### **Step 5.3: Connecting the World (Map Transitions)**

**Objective:** To allow the player to seamlessly move from one map to another by walking through doorways or designated transition zones.

**Conceptual Plan:** We will use Tiled's Object Layer to define "transition zones." When the player's sprite collides with one of these zones, the game will be instructed to unload the current map, load a new one, and place the player at a specific spawn point on the new map.

**Actionable Steps:**
1.  **Define Transition Objects in Tiled:**
    *   Open your maps in Tiled. In the `Objects` layer, create a rectangle object over each doorway.
    *   Select the object and add **Custom Properties** to it:
        *   `string targetMap` (e.g., "campus.json")
        *   `int targetX` (the X coordinate to spawn at on the new map)
        *   `int targetY` (the Y coordinate to spawn at on the new map)

2.  **Create a World Manager:**
    *   Create a high-level class, `WorldManager`.
    *   This class will be responsible for holding the `currentMap` object.
    *   Create a function `void WorldManager::SwitchMap(string targetMap, int targetX, int targetY)`.

3.  **Implement the `SwitchMap` Logic:**
    *   The `SwitchMap` function will:
        1.  Delete the `currentMap` object to free its memory.
        2.  Create a new `Map` object.
        3.  Call `newMap->LoadMapFromFile(targetMap)`.
        4.  Set the player's position to `targetX` and `targetY`.

4.  **Implement the Transition Check:**
    *   In your `Player::Update()` function, after you've calculated the player's new position but before you render:
        1.  Check if the player's new position rectangle is colliding with any of the "transition" objects on the current map.
        2.  If a collision is detected, get that object's custom properties (`targetMap`, `targetX`, `targetY`) and call `worldManager.SwitchMap()` with them.

**Verification Test:** You can start in the hostel room, walk out the door, and the screen should fade or cut to the campus map, with your character appearing at the entrance. You can then walk to another transition zone on the campus map to return to the hostel.

---

#### **Step 5.4: Populating the World (Placing Objects & NPCs)**

**Objective:** To fill your newly created maps with all the interactive elements and characters you built in the previous phases.

**Conceptual Plan:** This step leverages the `Object Layer` and the parser we built in Step 5.1. We will go back into Tiled and visually place every single interactive element, giving them properties that our game code can understand and use to bring them to life.

**Actionable Steps:**
1.  **Update the Map Parser:**
    *   Expand your `LoadMapFromFile` function. When it finds the `Objects` layer, it needs to loop through all the objects in it.
    *   For each object, it will read its **name** or a custom **type** property.
    *   `if (type == "NPC")`: Create a new `NPC` object using the object's position and its custom properties (e.g., `eventId`, `spriteFile`). Add it to a list of NPCs for the map.
    *   `if (type == "Interactable")`: Create a new `Interactable` object using its position and `eventId` property. Add it to a list of interactables for the map.

2.  **Place Everything in Tiled:**
    *   Go through each of your maps in Tiled.
    *   In the `Objects` layer, place an object for every single interactive thing: every bed, desk, poster, vending machine. Give each one the correct custom properties (e.g., `type="Interactable"`, `eventId="EV_SLEEP_PROMPT"`).
    *   Place objects to represent your NPCs. Give them properties like `type="NPC"`, `eventId="EV_PROFESSOR_GREETING"`, `spriteFile="professor_sprite.png"`.

**Final Verification Test for Phase 5:** Your game world is now explorable and populated. You can walk from the hostel to the campus. The hostel room contains a bed that you can interact with. The campus has a professor NPC standing there who you can talk to. The world feels like a real, cohesive place, ready for the gameplay systems of the next phase.