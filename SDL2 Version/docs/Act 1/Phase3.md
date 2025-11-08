Excellent. The world is interactive. Now, we inject the "simulation" into this life-sim. Phase 3 is about creating the underlying systems that give the player's actions meaning and consequences. We are building the engine of cause and effect.

As your Lead Designer, this is the blueprint for Phase 3. We are implementing the character's internal state (stats), the rules that govern that state (events), and the conditions for failure (health system).

---

### **Phase 3 Blueprint: The Simulation Engine**

**Your Goal for This Week:** To implement a complete character stat system. The player will have core attributes (Clout, Logic, Sanity) and secondary resources (Energy, Money) that are visible on a HUD. You will build a simple but powerful event system that can read instructions from a file to modify these stats. Finally, you will implement the health and failure system, where negative outcomes can lead to hospitalization and eventually a Game Over.

---

#### **Step 3.1: The Character Stats System & HUD**

**Objective:** To define and manage all the numerical attributes that represent the player character's state.

**Conceptual Plan:** We will expand our `Player` class to include all the stats from our Master Plan. Then, we will create a persistent Head-Up Display (HUD) to make this information constantly visible to the player.

**Actionable Steps:**
1.  **Expand the Player Class:**
    *   In your `Player.h` file, add the following member variables to the `Player` class:
        *   **Primary Stats:** `int clout;`, `int logic;`, `int sanity;`
        *   **Secondary Stats:** `int money;`, `float energy;`, `float maxEnergy;`
    *   Initialize these with starting values in the `Player` constructor (e.g., `clout = 5`, `logic = 5`, `sanity = 100`, `money = 500`, `energy = 100.0f`, `maxEnergy = 100.0f`).

2.  **Design the HUD:**
    *   In your `UIManager` or a dedicated `HUD` class, create a `RenderHUD()` function.
    *   This function will be responsible for drawing the static UI elements that are always on screen.
    *   It should use your `RenderText` function to display the current values of `Clout`, `Logic`, `Sanity`, and `Money`.
    *   **Implement an Energy Bar:** Just like the HP bar from the original plan, this will be a visual representation of the `energy / maxEnergy` ratio. Draw a background rectangle and then a foreground rectangle on top of it, with the width scaled by the energy percentage.

3.  **Integrate HUD into the Main Loop:**
    *   In your main game's `render` function, make sure to call `uiManager.RenderHUD(player)` every frame. It should be one of the last things drawn so it appears on top of everything else except for pop-up dialogue boxes.

**Verification Test:** When you run the game, you can see your character's stats (Clout, Logic, Sanity, Money) and a full energy bar displayed in a fixed position on the screen (e.g., the top-left corner). These values are static for now, but they are visible.

---

#### **Step 3.2: Event System V1 (The Rule Engine)**

**Objective:** To create a system that can read simple, script-like commands from a text file and use them to modify the player's stats.

**Conceptual Plan:** This is the heart of the simulation. Instead of hard-coding `player.money -= 10;` in our C++ code, we will create an `EventManager` that can read a line like `SUBTRACT_MONEY 10` from a file. This makes our game incredibly flexible and easy to expand.

**Actionable Steps:**
1.  **Define the Event File Format:**
    *   Create a new folder: `assets/events`.
    *   Create a test event file, `EV_BUY_COFFEE.txt`. Inside, write simple commands, one per line:
        ```
        DIALOGUE "You bought a coffee. It's surprisingly decent."
        SUBTRACT_MONEY 20
        ADD_STAT energy 15
        PLAY_SOUND sfx_cash_register.wav
        ```

2.  **Create the Event Manager:**
    *   Create `EventManager.h` and `EventManager.cpp`.
    *   Create a function: `void TriggerEvent(string eventId, Player& player, UIManager& uiManager, AudioManager& audioManager)`. This function needs references to the other managers so it can command them.

3.  **Implement the Event Parser:**
    *   Inside `TriggerEvent()`, the logic is:
        1.  Open the corresponding event file (e.g., `assets/events/EV_BUY_COFFEE.txt`).
        2.  Read the file line by line.
        3.  For each line, parse the command and its arguments. A simple way is to find the first space. The word before it is the command, the rest is the argument.
        4.  Use an `if-else if` chain or a `switch` statement on the command string:
            *   `if (command == "DIALOGUE")`: Call `uiManager.ShowDialogue(argument)`.
            *   `if (command == "ADD_STAT")`: Parse the argument to get the stat name and the value. Update `player.logic += value;` etc.
            *   `if (command == "SUBTRACT_MONEY")`: Parse the value and update `player.money -= value;`.
            *   `if (command == "PLAY_SOUND")`: Call `audioManager.PlaySoundEffect(argument)`.

4.  **Integrate with the Interaction System:**
    *   Go back to your interaction logic from Phase 2. When the player interacts with an object, instead of directly calling `uiManager.ShowDialogue()`, you will now call `eventManager.TriggerEvent(eventId, ...)` with the ID you get from the interactable object.

**Verification Test:** Create an "ATM" interactable object on your map with the event ID "EV_GET_MONEY". Create the corresponding `.txt` file with commands like `ADD_MONEY 500` and `DIALOGUE "You withdrew 500 rupees."`. When you interact with the ATM in-game, the dialogue should appear, and you should see the Money value on your HUD instantly increase.

---

#### **Step 3.3: Time-Based Logic & Energy Depletion**

**Objective:** To connect the stat system with the time system, making the world feel like a true simulation.

**Conceptual Plan:** The player's Energy should not be static. It should naturally decrease as they are awake, forcing them to rest. This creates a core gameplay loop of activity and recovery.

**Actionable Steps:**
1.  **Create a Constant for Energy Drain:**
    *   In your `Player` class or a global config file, define a constant like `const float ENERGY_DRAIN_PER_HOUR = 4.0f;`. This is a value you can easily tweak to change the game's difficulty.

2.  **Link Energy Drain to the Clock:**
    *   In your `TimeManager::Update()` function, every time the in-game `hour` ticks over, you need to tell the player to lose energy.
    *   The best way to do this is with a simple callback or by having the `Update` function return a flag. For example, `bool newHourPassed = timeManager.Update(deltaTime);`.
    *   In your main game loop, if `newHourPassed` is true, call a new function `player.DrainEnergy(ENERGY_DRAIN_PER_HOUR)`.

3.  **Clamp Stat Values:**
    *   In your player stat modification functions (like `DrainEnergy` or when an event adds energy), make sure to "clamp" the values. This means ensuring they don't go above the maximum or below zero.
    *   For example: `energy = std::max(0.0f, energy);` and `energy = std::min(maxEnergy, energy);`.

**Verification Test:** Start the game and watch the HUD. As the in-game clock progresses and the hours tick by, you should see the player's energy bar slowly but noticeably decrease.

---

#### **Step 3.4: The Health & Failure System**

**Objective:** To introduce a clear failure state to the game, giving the player's choices weight and risk.

**Conceptual Plan:** We will track the number of times the player has been "incapacitated" (hospitalized). This will be a global counter. Certain events will now have the power to trigger a hospitalization, which applies penalties and increments the counter. Reaching the limit is a Game Over.

**Actionable Steps:**
1.  **Add a Hospitalization Counter:**
    *   In your main `Game` class or `Player` class, add a member variable: `int hospitalizationCount = 0;`.
    *   Add a constant: `const int MAX_HOSPITALIZATIONS = 5;`.

2.  **Create the Hospitalization Function:**
    *   Create a function in your `Game` class called `void GoToHospital()`.
    *   This function's logic should:
        1.  Increment `hospitalizationCount`.
        2.  Check `if (hospitalizationCount >= MAX_HOSPITALIZATIONS)`. If so, trigger a `GameOver()` function.
        3.  Deduct a significant amount of money and time (e.g., `player.money -= 1000;`, `timeManager.AdvanceDay();`).
        4.  Move the player's position back to their bed in the hostel room.
        5.  Refill the player's energy to max.
        6.  Show a dialogue message like `uiManager.ShowDialogue("You woke up in the hospital... again. That was a close call.")`.

3.  **Create the `GameOver` State:**
    *   Create a new game state (e.g., in an `enum GameState { PLAYING, GAME_OVER }`).
    *   The `GameOver()` function should change the game state.
    *   In your main loop, if the state is `GAME_OVER`, you should stop updating the player and the world, and instead just render a "GAME OVER" message in the center of the screen.

4.  **Create a "HOSPITALIZE" Event Command:**
    *   Go back to your `EventManager`. Add a new command: `HOSPITALIZE`.
    *   When the parser sees this command, it should call the `game.GoToHospital()` function you just created.
    *   Create a new test event file for a dangerous interaction, e.g., `EV_TOUCH_LIVE_WIRE.txt`, and put the `HOSPITALIZE` command in it.

**Final Verification Test for Phase 3:** Interact with the "live wire" object. The game should penalize you, move you back to your bed, and you should see a hospitalization message. The next time you trigger it, the same thing should happen. Trigger this event 5 times. On the fifth time, the game should stop, and a "GAME OVER" screen should appear.