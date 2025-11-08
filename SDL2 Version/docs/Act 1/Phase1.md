Of course. Let's begin Act I. This is the most crucial phase from a technical standpoint. We are building the concrete and steel foundation upon which the entire game will rest. Get this right, and everything that follows becomes easier.

As your Lead Designer, this is the detailed, no-code blueprint for Phase 1. Follow these steps methodically.

---

### **Phase 1 Blueprint: The World Engine**

**Your Goal for This Week:** To create a running SDL2 application that displays a top-down, tile-based map. A player sprite will be visible on this map, and you will be able to move it around in four directions. The player will be stopped by walls. A single background music track will play continuously. There will be **zero** gameplay logic.

---

#### **Step 1.1: Assembling the Graphics Workshop (SDL2 Setup & Linking)**

**Objective:** To get a basic, empty window to appear on your screen and close correctly. This is the "Hello, World!" of this project and the most critical technical hurdle.

**Conceptual Plan:** You must first configure your project to use the SDL2 libraries, which allow your C++ code to talk to your computer's graphics and audio hardware. We will initialize these libraries, ask the operating system for a window, and create a "renderer" which is our tool for drawing inside that window.

**Actionable Steps:**
1.  **Install Libraries:** Install the development libraries for **SDL2 (Core)**, **SDL2_image** (for loading `.png` files), and **SDL2_mixer** (for audio).
2.  **Configure Your Project:** Set up your compiler/IDE to correctly link against these three libraries. This involves telling it where their header files (`.h`) and library files (`.lib`, `.so`, `.a`) are located.
3.  **Initialize Systems:** In your `main.cpp`, at the very beginning, you must call the initialization functions for SDL's video and audio subsystems, as well as the SDL_image and SDL_mixer libraries.
4.  **Create Window & Renderer:** Create a main `SDL_Window` (e.g., 1024x768 resolution) and a main `SDL_Renderer` from that window.
5.  **Implement the Game Loop:** Create the standard event-driven game loop. This loop will:
    *   Check for SDL events (like the user clicking the window's 'X' button).
    *   Clear the renderer to a solid color (e.g., black) each frame.
    *   Present the renderer to the window to show the changes.
    *   The loop should continue until an event tells it to quit.

**Verification Test:** You can compile and run your program. It **must** open a blank window of the correct size. The window **must** close cleanly without errors when you click the 'X' button or have it close when you press the ESC key. **Do not proceed until this works flawlessly.**

---

#### **Step 1.2: Preparing Your Art & Audio Assets**

**Objective:** To create the placeholder visual and audio assets our engine will need to function.

**Conceptual Plan:** An engine is useless without content. We need to create the raw materials: an image for the player, an image containing our world's building blocks (a "tileset"), and a music file.

**Actionable Steps:**
1.  **Create Folder Structure:** Inside your project, create an `assets` folder. Inside `assets`, create `sprites`, `tilesets`, and `music`.
2.  **Create Player Sprite:** In a graphics program, create a simple 32x32 pixel top-down character. Save it as `player.png` in the `assets/sprites/` folder.
3.  **Create World Tileset:** Create a new image, `world_tileset.png`, in `assets/tilesets/`. Inside this file, draw at least two 32x32 pixel tiles side-by-side: a simple floor pattern and a simple wall pattern.
4.  **Acquire Background Music:** Find a royalty-free `.mp3` or `.ogg` file and place it in `assets/music/`, naming it `theme.mp3`.

---

#### **Step 1.3: The World Map System**

**Objective:** To represent your game world as a grid and render it to the screen using your tileset.

**Conceptual Plan:** A 2D game map is simply a 2D array of numbers. Each number represents a different tile from your tileset. We will create a `Map` class that holds this array and contains the logic for drawing the corresponding tiles in the correct positions.

**Actionable Steps:**
1.  **Create the Map Class:** Create `Map.h` and `Map.cpp`.
2.  **Define the Level Data:** Inside the `Map` class, create a hard-coded 2D `int` array. Use `0` for floor tiles and `1` for wall tiles to lay out a simple room with walls.
3.  **Load the Tileset:** In the `Map`'s constructor, use SDL_image to load `world_tileset.png` into an `SDL_Texture`.
4.  **Implement the Render Logic:**
    *   Create a `Map::Render()` function.
    *   This function will use two nested `for` loops to iterate through your `levelData` grid.
    *   Inside the loops, it will calculate two things for each tile:
        *   The **Source Rect:** Which 32x32 square to cut out from the tileset texture. (e.g., if the grid number is `1`, cut out the second tile from the left).
        *   The **Destination Rect:** Where on the screen to draw that tile. (e.g., grid position `[row][col]` draws at screen position `x = col * 32`, `y = row * 32`).
    *   Call `SDL_RenderCopy()` with these two rectangles to draw the tile.

**Verification Test:** When you run your program, the blank window now shows a complete, static game map built from your floor and wall tiles.

---

#### **Step 1.4: The Player & Movement System**

**Objective:** To display the player sprite and allow the user to move it with keyboard input.

**Conceptual Plan:** We will create a `Player` class to manage its own sprite, position, and movement logic. The main game loop will inform the `Player` object which keys are being pressed, and the `Player` object will update its own position accordingly.

**Actionable Steps:**
1.  **Create the Player Class:** Create `Player.h` and `Player.cpp`. It should store its sprite texture and its `x, y` coordinates.
2.  **Load the Sprite:** In the `Player` constructor, load `player.png`.
3.  **Implement Rendering:** Create a `Player::Render()` function that draws the player texture at its current `x, y` position.
4.  **Implement Input & Update Logic:**
    *   Create a `Player::Update()` function. This function will read the current keyboard state using `SDL_GetKeyboardState()`.
    *   Based on which arrow key is held down (e.g., `if (keyboardState[SDL_SCANCODE_UP])`), it will modify its `x, y` coordinates. Use a `speed` variable to control how fast it moves.

**Verification Test:** Your player sprite now appears on the map. You can move the sprite in all four directions using the arrow keys. For now, the sprite will pass straight through walls.

---

#### **Step 1.5: Collision Detection & Audio Integration**

**Objective:** To make the world feel solid by preventing movement through walls, and to add atmosphere with background music.

**Conceptual Plan:** Before the player moves, they will ask the map if the tile they *want* to move into is solid. If the map says yes, the move is cancelled. For audio, we will simply load our music file and tell it to play.

**Actionable Steps:**
1.  **Implement Collision Logic in Map:**
    *   In your `Map` class, create a function `bool IsSolidTileAt(int x, int y)`.
    *   This function will take screen coordinates, convert them to grid coordinates (e.g., `gridX = x / 32`), and check the `levelData` array. It returns `true` if the tile at that position is a `1` (a wall).
2.  **Update Player Movement:**
    *   Modify the `Player::Update()` function.
    *   Before changing the player's position, calculate the `nextX` and `nextY`.
    *   Call the `map.IsSolidTileAt(nextX, nextY)` function.
    *   **Only** update the player's position if the function returns `false`. You will need to do this check separately for horizontal and vertical movement to prevent getting stuck on corners.
3.  **Implement Audio:**
    *   Create a simple `AudioManager` class.
    *   In its constructor, initialize SDL_mixer using `Mix_OpenAudio()`.
    *   Create a function to load and play music using `Mix_LoadMUS` and `Mix_PlayMusic`.
    *   In your `main` function, after initialization, call your audio manager to play `theme.mp3` on a loop (`-1` as a parameter in `Mix_PlayMusic`).

**Final Verification Test for Phase 1:** You can move your player sprite around the map. When the sprite touches a wall tile, it stops and cannot pass through. The background music is playing continuously. If you have achieved this, the World Engine is complete and robust.