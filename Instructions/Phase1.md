### **Phase 1 Blueprint: Core Engine & Systems (No-Code Instructions)**

**Your Goal for This Week:** Create a C++ program that can open a window-like terminal, read game data from a text file, and hold a player's state in memory. It won't be a "game" yet, but it will be a working, data-driven application.

---

#### **Step 1.1: Setting Up Your Workshop (Environment and Project Structure)**

**Objective:** To create an organized folder structure for your project. Good organization now will save you massive headaches later.

**Conceptual Plan:** Your project will have three main parts: your source code (`.cpp` files), your header files (`.h` files), and your game content (`.txt` files). We need to create separate folders for each.

**Actionable Steps:**
1.  Create a main project folder called `MemeLife`.
2.  Inside `MemeLife`, create three sub-folders:
    *   `src`: This is where all your C++ implementation files (`.cpp`) will go.
    *   `include`: This is where all your C++ header/declaration files (`.h`) will go.
    *   `assets`: This is where all non-code game content will live. For now, it will hold our event files.
3.  Inside the `assets` folder, create another folder called `events`.
4.  Create your main source file, `main.cpp`, inside the `src` folder.
5.  Set up your build system (a `Makefile` or `CMakeLists.txt` file in the main `MemeLife` directory) to know that it should look for `.h` files in `include` and `.cpp` files in `src`.

**Verification Test:** You should be able to compile and run your empty `main.cpp` without any errors.

---

#### **Step 1.2: Taming the Terminal (Terminal Control Module)**

**Objective:** To gain low-level control of the terminal so we can treat it like a graphics canvas. Standard `cout` is not enough for a game.

**Conceptual Plan:** We will create a dedicated "module" (a pair of `.h` and `.cpp` files) whose only job is to manage the terminal. This module will use special commands called **ANSI escape codes** to do things like clear the screen, change text color, and move the cursor. We also need a way to get keyboard input instantly, without the user having to press "Enter."

**Actionable Steps:**
1.  Create two new files: `Terminal.h` (in `include`) and `Terminal.cpp` (in `src`).
2.  In `Terminal.h`, declare the following functions. You don't need to write the code for them yet, just declare them:
    *   `void ClearScreen()`: Will be responsible for wiping all text from the terminal.
    *   `void MoveCursor(int x, int y)`: Will move the terminal's text cursor to a specific coordinate.
    *   `void SetColor(int colorCode)`: Will change the color of any text printed after it is called.
    *   `char GetImmediateInput()`: This is the special one. It will wait for a single keypress from the user and return it immediately.
3.  Now, in `Terminal.cpp`, implement the logic for these functions. You will need to research how to:
    *   Print the ANSI escape code for clearing the screen (`\033[2J`).
    *   Print the ANSI escape code for moving the cursor (`\033[<L>;<C>H`, where L and C are line and column).
    *   Print ANSI escape codes for colors (e.g., `\033[31m` for red).
    *   For `GetImmediateInput`, research how to use the `<conio.h>` library on Windows (`_getch()`) or the `<termios.h>` library on Linux/macOS to enable non-canonical, no-echo input.

**Verification Test:** Write a temporary `main` function that calls these functions. It should:
1.  `ClearScreen()`.
2.  `MoveCursor(10, 5)`.
3.  `SetColor()` to red.
4.  Print the word "TEST".
5.  Call `GetImmediateInput()` to wait for a keypress.
6.  The program should close the instant you press any key.

---

#### **Step 1.3: Designing the Blueprints (Core Data Structures)**

**Objective:** To define what a "Player" or a "Game Event" is using `structs`. This is how our game will organize its data in memory.

**Conceptual Plan:** Before we can store data, we need to create "templates" for it. A `struct` in C++ is a perfect way to group related variables together. We will create a single header file to hold all of these core data templates.

**Actionable Steps:**
1.  Create a new file named `GameData.h` in your `include` folder.
2.  Inside this file, define the following `structs`:
    *   **`Stats` struct:** This will contain integer variables like `clout`, `logic`, `sanity`, `hp`, and `maxHp`.
    *   **`Player` struct:** This will contain a `string` for the player's `name` and `memeType`, integers for `level` and `xp`, and it will contain a `Stats` object (the struct you just defined above) to hold all its stats.
    *   **`Choice` struct:** This will represent a single option for the player. It should contain a `string` for its `displayText` and another `string` for its `outcomeId` (which tells the game which event to load next).
    *   **`GameEvent` struct:** This will be the template for every situation in the game. It should contain a `string` for its unique `id`, a `string` for its `descriptionText`, a list/vector of `Choice` objects, and a list/vector of strings to hold any associated ASCII art.

**Verification Test:** Include `GameData.h` in your `main.cpp`. The program should still compile successfully. This confirms your definitions are syntactically correct.

---

#### **Step 1.4: Building the Engine (Game Loop and State Manager)**

**Objective:** To create the "heartbeat" of the game—an infinite loop that will eventually run all our game logic.

**Conceptual Plan:** Every game runs on a loop that does three things over and over: check for user input, update the game's state, and render everything to the screen. We'll create a central `Game` object to manage this loop and hold the current state (like the player's data and which event is active).

**Actionable Steps:**
1.  In your `main.cpp`, create a `Game` class or struct.
2.  Give the `Game` object two member variables:
    *   A `Player` object (using the `Player` struct you just made).
    *   A boolean flag called `isRunning`, initialized to `true`.
3.  Create a main method within the `Game` object called `Run()`.
4.  Inside `Run()`, create a `while` loop that continues as long as `isRunning` is true.
5.  Inside the loop, for now, just print the player's current HP to the screen.
6.  In your `main` function, create an instance of the `Game` object and call its `Run()` method.

**Verification Test:** When you run the program, it should start printing the player's default HP to the screen over and over, filling the console. This confirms your game loop is running.

---

#### **Step 1.5: Connecting to the Outside World (The Asset Parser)**

**Objective:** To make our game read its content from a text file instead of having it hard-coded. This is the most crucial step for making your game expandable.

**Conceptual Plan:** We will write a dedicated "loader" function. Its job is to open a specific `.txt` file, read it line by line, and fill a `GameEvent` struct with the data it finds. We'll use simple prefixes (like `DESC:`) to identify what each line of the file means.

**Actionable Steps:**
1.  In your `assets/events` folder, create a new text file named `01_start.txt`.
2.  Inside this file, type the following, exactly:
    ```
    ID: 01_start
    DESC: You awaken. A blinking cursor is the only thing you can see. Your journey begins.
    CHOICE: Embrace the meme. | 02_first_choice
    CHOICE: Question reality. | 03_logic_choice
    ```
3.  Create two new files: `AssetLoader.h` (in `include`) and `AssetLoader.cpp` (in `src`).
4.  In `AssetLoader.h`, declare a function: `GameEvent LoadEvent(string eventId)`.
5.  In `AssetLoader.cpp`, implement this function. The logic should be:
    *   It takes an ID like `"01_start"`.
    *   It opens the file at the path `assets/events/01_start.txt`.
    *   It creates a new, empty `GameEvent` object.
    *   It reads the file line by line. For each line, it checks the prefix:
        *   If the line starts with `ID:`, it saves the rest of the line to the event's `id` variable.
        *   If it starts with `DESC:`, it saves the rest to the `descriptionText`.
        *   If it starts with `CHOICE:`, it splits the line at the `|` character. The first part becomes the `displayText` of a new `Choice` object, and the second part becomes the `outcomeId`. It then adds this `Choice` to the event's list of choices.
    *   Finally, it returns the fully populated `GameEvent` object.
6.  Modify your `Game` object. Give it a `GameEvent` member variable called `currentEvent`. In the beginning of your `Run()` method, call `LoadEvent("01_start")` and store the result in `currentEvent`.
7.  In your main loop's `render` step, instead of printing the player's HP, print the `descriptionText` from the `currentEvent`.

**Verification Test:** Run the program. It should clear the screen, display the text "You awaken. A blinking cursor is the only thing you can see. Your journey begins.", and then loop.

If you complete this final test, you have successfully built the entire foundation of the game engine. You are now ready for Phase 2.