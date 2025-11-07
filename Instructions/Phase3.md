### **Phase 3 Blueprint: The Heat of Battle (No-Code Instructions)**

**Your Goal for This Week:** To implement a fully functional, turn-based combat system. By the end of this phase, an event in the game will be able to trigger a battle, the player will be able to fight an enemy using meme-based moves, and the outcome of the battle will determine what happens next.

---

#### **Step 3.1: Defining Combat (Battle Framework and Data)**

**Objective:** To create the data structures and state management necessary for a battle. Before we can code a fight, we must define what a "fight" is in our game's terms.

**Conceptual Plan:** A battle is a special state the game can be in. We need a "Battle Manager" to control this state. This manager will need to know about the player, the enemy it's fighting, and whose turn it is. We also need to define what an "Enemy" and a "Battle Move" are in our `GameData.h` file.

**Actionable Steps:**
1.  **Expand Your Data Structures (`GameData.h`):**
    *   **Create the `BattleMove` struct:** This will define an attack. It needs a `string name` ("Logic Bomb"), an `int power` (base damage), and a `string targetStat` ("Sanity", "Logic", etc.).
    *   **Create the `Enemy` struct:** This is similar to the `Player` struct. It needs a `string name` ("Karen"), a `vector<string>` for its ASCII art, a `Stats` object, and a `vector<BattleMove>` for the moves it knows.

2.  **Create the Battle Manager:**
    *   Create two new files: `BattleManager.h` (in `include`) and `BattleManager.cpp` (in `src`).
    *   Inside the `BattleManager` class, create an `enum` to track the current state of the battle: `enum BattleState { STARTING, PLAYER_TURN, ENEMY_TURN, WIN, LOSE }`.
    *   Give the `BattleManager` member variables to hold the `Player` object, the current `Enemy` object, and the current `BattleState`.

3.  **Teach the Game to Start a Battle:**
    *   Modify your event `.txt` file format. A choice outcome should now be able to trigger a battle. Example: `CHOICE: Refuse to back down. | BATTLE | karen_encounter_1`.
    *   Update your `AssetLoader` to handle enemies. Create an `assets/enemies` folder. Enemy files (`karen_encounter_1.txt`) will define their stats and moves, similar to how you define events.
    *   In your `Game` class's `update()` function, when a choice with a "BATTLE" outcome is selected, it should:
        1.  Load the specified enemy data.
        2.  Initialize your `BattleManager` with the player and the newly loaded enemy.
        3.  Change the main game state from `EXPLORING` to `IN_BATTLE`.

**Verification Test:** Play the game up to an event that triggers a battle. When you select the choice, the game should stop displaying the event and could, for now, just print "BATTLE HAS BEGUN!" to the console and pause. This confirms the entire state transition is working correctly.

---

#### **Step 3.2: Visualizing the Battlefield (Battle UI and Rendering)**

**Objective:** To create a dedicated screen layout for combat that clearly displays all necessary information.

**Conceptual Plan:** We'll expand our `Renderer` module with a new function specifically for drawing the battle screen. This keeps our code organized. This screen will be fundamentally different from the event screen, showing two combatants, their health, and a menu of actions.

**Actionable Steps:**
1.  **Create the Battle Rendering Function:**
    *   In `Renderer.h`, declare a new function: `void DrawBattleScreen(const Player& player, const Enemy& enemy, const vector<string>& battleLog)`.
    *   In `Renderer.cpp`, implement this function. It should:
        *   Draw the player's ASCII art on one side of the screen (e.g., the left).
        *   Draw the enemy's ASCII art on the other side (e.g., the right).
        *   Draw the player's HP bar and stats below their art.
        *   Draw the enemy's HP bar below their art.
        *   Draw a "Move Selection" box at the bottom, listing the names of the moves in the player's `BattleMove` list, numbered '1', '2', etc.
        *   Draw another box for the "Battle Log," which will display commentary on the fight. For now, this will be empty.

2.  **Integrate into the Main Render Loop:**
    *   In your main `Game` class's `render()` function, you must now add a condition.
    *   `if (gameState == IN_BATTLE)`, call `renderer.DrawBattleScreen()`.
    *   `else`, call the existing `renderer.DrawEvent()` function.

**Verification Test:** When a battle begins, the screen should now flip from the event view to the new battle layout. You should see the player and enemy sprites, both with full HP bars, and a list of the player's moves. The game will still be paused, but the entire interface is now ready.

---

#### **Step 3.3: The Dance of Combat (Turn-Based Logic Implementation)**

**Objective:** To implement the rules of combat. This is where the battle becomes an interactive, playable system.

**Conceptual Plan:** We will build the logic for our state machine inside `BattleManager::update()`. The player's input will now select a move. The `update` function will process the move, calculate damage, check for a winner, and then trigger the enemy's turn.

**Actionable Steps:**
1.  **Implement the Player's Turn:**
    *   In your `Game` class's `handleInput()`, if the state is `IN_BATTLE`, the number keys should correspond to selecting a move from the menu. Store this choice.
    *   In `BattleManager::update()`, if the `BattleState` is `PLAYER_TURN` and a move has been selected:
        *   Get the chosen `BattleMove`'s `power`.
        *   Add a small amount of randomness (e.g., `power +/- 10%`) to the damage.
        *   Subtract the final damage from the enemy's `hp`.
        *   Add a descriptive string like "You used Logic Bomb!" to a `vector<string>` battle log.
        *   Change the `BattleState` to `ENEMY_TURN`.

2.  **Implement the Enemy's Turn:**
    *   In `BattleManager::update()`, if the `BattleState` is `ENEMY_TURN`:
        *   Implement a simple AI: have the enemy choose a random move from its move list.
        *   Perform the same damage calculation against the player's `hp`.
        *   Add a string like "Karen used Outraged Scream!" to the battle log.
        *   After a short, artificial delay (so the turn doesn't happen instantly), change the `BattleState` back to `PLAYER_TURN`.

3.  **Implement Win/Loss Conditions:**
    *   After any damage is dealt (to either the player or the enemy), add a check:
    *   `if (enemy.hp <= 0)`, change the `BattleState` to `WIN`.
    *   `if (player.hp <= 0)`, change the `BattleState` to `LOSE`.
    *   In the `WIN` or `LOSE` states, display a victory/defeat message in the battle log and wait for a keypress. Once pressed, change the main `gameState` back to `EXPLORING` and load the appropriate next event (e.g., `karen_victory` or `karen_defeat`).

4.  **Update the Battle Log Display:**
    *   In `Renderer::DrawBattleScreen()`, make it draw the last 3-4 strings from the battle log vector, creating a running commentary of the fight.

**Verification Test:** You can now play a complete battle. You can select moves, see their effect on the enemy's HP bar, and read the commentary in the log. The enemy will attack you back. You can win the battle by depleting the enemy's health, or lose if your health runs out.

---

#### **Step 3.4: Adding the Flair (Basic ASCII Animations)**

**Objective:** To provide simple, satisfying visual feedback for actions in combat.

**Conceptual Plan:** An animation is just a series of text "frames" drawn in sequence with a delay. We will create a system to load these frames from a file and a "player" to display them, temporarily pausing the game logic to do so.

**Actionable Steps:**
1.  **Design the Animation Asset:**
    *   Create a new folder: `assets/animations`.
    *   Create a text file inside called `beam.txt`.
    *   In this file, define the frames of a simple projectile animation, separated by a unique delimiter. For example:
        ```
        *-->
        ---FRAME---
         *-->
        ---FRAME---
          *-->
        ```

2.  **Create an Animation System:**
    *   Update `AssetLoader` to have a function that can read one of these animation files and store it as a list of frames.
    *   Create a new `AnimationPlayer` module (`.h` and `.cpp`).
    *   The `AnimationPlayer` will have one main function: `void Play(animation_data, start_x, start_y)`.
    *   The logic for `Play()` is a loop that:
        1.  Clears the area where the animation will play (by drawing spaces).
        2.  `MoveCursor` to the correct position.
        3.  Draws the current frame.
        4.  Waits for a very short duration (e.g., 50 milliseconds).

3.  **Integrate into Combat:**
    *   In `BattleManager::update()`, when the player or enemy selects a move, before calculating the damage, call the `animationPlayer.Play()` function. You can pass it the coordinates of the attacker as the starting point.

**Verification Test:** When a move is used in combat, a simple ASCII effect should play out on screen (e.g., a "beam" travels from the attacker to the defender). The battle should pause during the animation and resume immediately after, before the damage numbers are applied. This will make the combat feel significantly more impactful and polished.