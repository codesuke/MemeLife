### **Phase 2 Blueprint: Bringing the World to Life (No-Code Instructions)**

**Your Goal for This Week:** Transform the application into a playable "choose your own adventure" game. You will build a visual interface (HUD), make player choices functional, display ASCII art, and implement a way to save and load progress.

---

#### **Step 2.1: The Game's Dashboard (ASCII User Interface & HUD)**

**Objective:** To give the terminal a professional, game-like layout. The player needs to see their vital stats at all times.

**Conceptual Plan:** We will create a dedicated `Renderer` module. Its sole job is to draw things on the screen. We'll use it to draw a border around the play area and a persistent "Heads-Up Display" (HUD) that shows the player's current status. This HUD will be redrawn every single frame to ensure it's always visible and up-to-date.

**Actionable Steps:**
1.  **Create the Renderer Module:**
    *   Create two new files: `Renderer.h` (in `include`) and `Renderer.cpp` (in `src`). This module will need to use your `Terminal` module to function.

2.  **Design Basic Drawing Tools:**
    *   In `Renderer.h`, declare a function: `void DrawBox(int x1, int y1, int x2, int y2)`.
    *   In `Renderer.cpp`, implement `DrawBox`. This function will use the ASCII box-drawing characters (like `|`, `-`, `+`) and your `Terminal::MoveCursor()` function to draw a rectangle on the screen between the two coordinate pairs.
    *   Also declare a simple `void DrawText(int x, int y, string text)` function.

3.  **Design the HUD:**
    *   In `Renderer.h`, declare a function: `void DrawHUD(const Player& player)`. It needs to take the player object as a parameter so it can access the stats.
    *   In `Renderer.cpp`, implement `DrawHUD`. This function will:
        *   Draw a smaller box at the top or bottom of the screen to contain the HUD information.
        *   Inside the box, display the player's name, Clout, Logic, and Sanity stats.
        *   **Implement the HP Bar:** This is a key visual. The logic is:
            *   Calculate the percentage of health the player has (`(float)hp / maxHp`).
            *   Determine the total width of the bar (e.g., 20 characters).
            *   Draw a number of solid characters (`#`) equal to `health_percentage * bar_width`.
            *   Fill the rest of the bar with empty characters (`-`). The result should look like `HP [#######---]`.

4.  **Integrate into the Game Loop:**
    *   In your main `Game` class, in your `render()` function, make the *first* call `Terminal::ClearScreen()`.
    *   Then, call your new `Renderer::DrawBox()` to draw the main game window border.
    *   Finally, call `Renderer::DrawHUD()` to draw the status display.

**Verification Test:** When you run the program, the screen should immediately clear and display a static, bordered window with a HUD showing the player's default stats (e.g., Clout: 10, HP: [##########]). The main area inside the border will be empty.

---

#### **Step 2.2: Making Choices Matter (Event & Dialogue System Integration)**

**Objective:** To connect the player's keyboard input to the game's logic, allowing them to navigate between events.

**Conceptual Plan:** We will now use the data we loaded in Phase 1. The `render()` function will display the current event's description and choices. The `handleInput()` function will listen for the player's choice. The `update()` function will process that choice, change the game's state, and load the next event.

**Actionable Steps:**
1.  **Displaying the Event:**
    *   In your `Renderer` module, create a new function: `void DrawEvent(const GameEvent& event)`.
    *   This function will draw the `event.descriptionText` inside your main game window.
    *   It will then loop through the `event.choices` vector. For each choice, it will print its number (e.g., "1. ") followed by the `choice.displayText`.

2.  **Handling Input:**
    *   In your `Game` class's `handleInput()` function, use your `Terminal::GetImmediateInput()` to listen for keypresses.
    *   If the key is '1', '2', '3', etc., store that number in a new member variable within your `Game` object, like `int playerInput = 1;`.

3.  **Updating the State:**
    *   This is the most important part. In your `Game` class's `update()` function, check if `playerInput` is not zero.
    *   If a choice was made:
        *   Get the corresponding `Choice` object from the `currentEvent`'s list of choices (remembering that vector indices are 0-based, so input '1' is index 0).
        *   Get the `outcomeId` string from that `Choice` object.
        *   Set a new `Game` member variable, `string nextEventId`, to this `outcomeId`.
        *   Reset `playerInput` to 0 so this logic doesn't run again next frame.
    *   Now, still in `update()`, check if `nextEventId` is not empty.
        *   If it's not, call your `AssetLoader::LoadEvent(nextEventId)` and replace your `currentEvent` with the new one.
        *   Clear the `nextEventId` string.

**Verification Test:** You should now be able to play through a sequence of events. Start the game, see the first event and its choices. Press '1'. The screen should update to show the description and choices for the event with the ID `02_first_choice`.

---

#### **Step 2.3: Visualizing the World (ASCII Sprite Handling)**

**Objective:** To display multi-line ASCII art within our game window.

**Conceptual Plan:** First, we'll teach our `AssetLoader` to read a text file that contains only ASCII art. Then, we'll teach our `Renderer` how to draw that art at a specific location. Finally, we'll modify our event files to tell the game which piece of art to display.

**Actionable Steps:**
1.  **Create an Art Asset:**
    *   Inside the `assets` folder, create a new folder called `ascii`.
    *   Inside `assets/ascii`, create a text file named `npc_idle.txt`.
    *   In this file, draw a simple character using text, for example:
        ```
         O
        /|\
        / \
        ```

2.  **Update the Asset Loader:**
    *   In `AssetLoader.h`, declare a new function: `vector<string> LoadAsciiArt(string artName)`.
    *   In `AssetLoader.cpp`, implement it. This function will open `assets/ascii/npc_idle.txt`, read each line of the file, and store it in a `vector<string>`. It will then return this vector.

3.  **Update the Renderer:**
    *   In `Renderer.h`, declare `void DrawAsciiArt(int x, int y, const vector<string>& art)`.
    *   In `Renderer.cpp`, implement it. The logic is to loop through the `art` vector. For each string in the vector, you will call `Terminal::MoveCursor(x, y + i)` (where `i` is the loop index) and print the string.

4.  **Update the Event System:**
    *   Modify your event `.txt` file format to include a new tag. For example, in `01_start.txt`:
        ```
        ID: 01_start
        ART: npc_idle
        DESC: You awaken...
        ...
        ```
    *   Update your `AssetLoader::LoadEvent` function to recognize the `ART:` tag. When it sees it, it should call `LoadAsciiArt()` with the art's name and store the resulting `vector<string>` inside the `GameEvent` object.
    *   Finally, in your `Renderer::DrawEvent` function, if the event has ASCII art, call `DrawAsciiArt()` to display it on the screen.

**Verification Test:** When an event that has an `ART:` tag is loaded, its corresponding ASCII art should appear in the main window along with the description text.

---

#### **Step 2.4: Remembering Your Journey (Save & Load System)**

**Objective:** To allow the player to quit the game and resume their progress later.

**Conceptual Plan:** Saving the game means writing the essential variables that define the player's current state to a file. Loading is the reverse process. The most important variables for now are the player's entire `Stats` object and the `currentEventId`.

**Actionable Steps:**
1.  **Design the Save Functions:**
    *   Create `SaveManager.h` and `SaveManager.cpp` or add these functions to your `Game` class.
    *   Define a function `void SaveGame(const Player& player, const string& currentEventId)`.
    *   Inside, this function will open a file named `memelife.sav`. It will then write the player's stats and the current event ID to the file in a simple, readable format, like:
        ```
        eventId: 02_first_choice
        clout: 12
        logic: 10
        sanity: 95
        ...
        ```

2.  **Design the Load Functions:**
    *   Define a function `bool LoadGame(Player& player, string& currentEventId)`. It should return `true` if loading succeeded and `false` if the save file doesn't exist.
    *   Inside, this function will try to open `memelife.sav`. If it succeeds, it will read each line, parse the key and the value, and update the `player` and `currentEventId` variables that were passed in by reference.

3.  **Integrate into a Main Menu:**
    *   Change your game's starting sequence. Before the main loop begins, display a menu: "1. New Game", "2. Continue".
    *   Wait for input. If the player chooses '1', start the game with a default `Player` object and the `"01_start"` event.
    *   If the player chooses '2', call `LoadGame()`. If it returns `false`, tell the player no save file was found and go back to the menu.
    *   Add a way to save. For now, you can make a specific choice in an event trigger the `SaveGame()` function.

**Verification Test:** Start a new game. Progress to the second or third event, which should have altered your stats. Trigger the save function. Close the program. Relaunch it, select "Continue". The game should load you into the correct event with your stats exactly as they were when you saved.