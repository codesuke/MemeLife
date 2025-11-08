### **Phase 4 Blueprint: Content, Polish & Replayability (No-Code Instructions)**

**Your Goal for This Week:** To fill the game with humorous content, implement a rewarding progression system, add features that encourage multiple playthroughs, and package the final product for release. This is where we add the soul.

---

#### **Step 4.1: The Content Creation Sprint**

**Objective:** To populate the game world with enough events, enemies, and art to create a full and satisfying gameplay loop.

**Conceptual Plan:** This step involves minimal new code. Instead, you will be a content creator, using the systems you've already built. Your job is to write the story, design the encounters, and draw the art in your text editors.

**Actionable Steps:**
1.  **Write the Narrative Arc:**
    *   Map out a simple story path on paper: The Beginning (NPC Student), The Middle (gaining influence, facing harder challenges), and The End (final confrontation).
    *   For each stage, brainstorm at least 10-15 event ideas. Examples: "You have to give a class presentation," "A wild Doomer appears and questions your life choices," "You discover a legendary meme format."
    *   Write the corresponding `.txt` files for each event in `assets/events/`, making sure they link to one another to form branching paths.

2.  **Design the Bestiary:**
    *   Create the definitive `.txt` files for 5-7 unique enemy archetypes in `assets/enemies/`.
    *   Give each enemy a distinct personality through their stats and moves.
        *   **Karen:** Low Logic, high HP, moves like "Scream" (lowers player Sanity) and "Demand Manager" (a powerful charge-up attack).
        *   **Sigma Male:** High Logic, immune to Clout-based attacks, uses moves like "Gaslight" and "Unsolicited Advice."
        *   **NPC Horde:** Low individual stats, but you fight 3-4 at once.
    *   Ensure each enemy file defines the XP reward for defeating them.

3.  **Create the Evolutionary Art:**
    *   Draw the ASCII art for each stage of the player's evolution in `assets/ascii/`.
    *   You need art for `npc_student.txt`, `sigma_teen.txt`, `gigachad.txt`, and `ascended_meme.txt`. This art will be swapped out as the player levels up.

**Verification Test:** You can play the game from the very beginning to a final ending without seeing any repeated content (unless intended). The playthrough should feel like a complete, albeit short, story. A single run should take about 15-20 minutes.

---

#### **Step 4.2: The Cycle of Growth (Character Progression & Endings)**

**Objective:** To make combat meaningful by rewarding the player with tangible growth and to provide a satisfying conclusion based on their playstyle.

**Conceptual Plan:** We need to implement the logic that grants XP, triggers a "Level Up" event, and checks the player's final stats to determine which ending they receive. This system is the core of the game's long-term reward loop.

**Actionable Steps:**
1.  **Implement the XP System:**
    *   In your `BattleManager`, when the battle state becomes `WIN`, get the XP value from the defeated enemy's data.
    *   Create a new function in your `Player` class called `void AddXP(int amount)`. Call this function after a victory.

2.  **Code the Level-Up Logic:**
    *   Inside `Player::AddXP()`, after adding the XP, check if `player.xp >= player.xpToNextLevel`.
    *   If it is, trigger a `LevelUp()` function.
    *   The `LevelUp()` function's logic should:
        *   Increase `player.level` by 1.
        *   Subtract `xpToNextLevel` from `xp`.
        *   Increase `xpToNextLevel` for the next level (e.g., multiply by 1.5).
        *   Grant stat increases (e.g., `maxHp += 10`, `logic += 2`).
        *   Change the `player.memeType` string to the next stage in the evolution.
        *   Potentially add a new, powerful `BattleMove` to the player's move list.
        *   Trigger a visual notification on screen: a "LEVEL UP!" message with a summary of the gains.

3.  **Implement the Multiple Endings:**
    *   Create three (or more) final event files in `assets/events/`: `ending_logic.txt`, `ending_clout.txt`, `ending_sanity.txt`. These files will contain the final narrative text describing the player's fate.
    *   After the final boss battle is won, instead of loading a normal next event, run a special check function.
    *   This function compares the player's final stats:
        *   `if (logic > clout && logic > sanity)`, load the `"ending_logic"` event.
        *   `if (clout > logic && clout > sanity)`, load the `"ending_clout"` event.
        *   `if (sanity is the lowest stat)`, load the `"ending_sanity"` event.

**Verification Test:** Defeat an enemy and see an "XP Gained!" message. After enough victories, the game should pause to show a "LEVEL UP!" screen, and you should see your stats and `memeType` permanently change in the HUD. Play the game to the end with a focus on Logic-based choices; see the Logic ending. Replay with a focus on Clout; see the Clout ending.

---

#### **Step 4.3: The Final Polish (Features for Fun & Replayability)**

**Objective:** To add the small, delightful features that make a game feel complete and encourage players to come back for more.

**Conceptual Plan:** This involves adding systems that introduce unpredictability (random encounters) and reward specific playstyles (achievements), as well as taking the time to balance the game's difficulty.

**Actionable Steps:**
1.  **Implement the Random Encounter Generator:**
    *   Instead of an event choice always leading to a single, specific next event, create "encounter pools."
    *   For example, a choice outcome could be `RANDOM_BATTLE_EASY`.
    *   When your `Game` logic sees this outcome, it will randomly pick one event ID from a predefined list of easy battle events. This prevents every playthrough from feeling identical.

2.  **Add Achievements/Titles:**
    *   In your `Game` or `Player` object, create a data structure to hold unlocked achievements (a `map` or `vector` of strings is perfect).
    *   Scatter checks for these achievements throughout your code.
        *   After a battle: `if (player.hp == 1) { unlockAchievement("Close Call"); }`.
        *   After a level up: `if (player.level == 10) { unlockAchievement("Meme Veteran"); }`.
    *   When an achievement is unlocked, display a notification on screen. You can also add a "View Achievements" option to the main menu.

3.  **Sprinkle in Humor & Easter Eggs:**
    *   Create a list of funny quotes or tips. Display a random one on screen when the game is loading.
    *   Create an extremely rare random encounter with a bizarre boss (like the "Rick Astley" idea).
    *   Implement a cheat code: on the main menu, if the user types a secret sequence of keys, unlock a "Meme God Mode" that boosts all stats.

4.  **Balance the Gameplay:**
    *   Play your game from start to finish. Multiple times.
    *   Ask critical questions: Is the first enemy too tough? Is leveling up too slow? Is there one move that is clearly better than all others?
    *   Adjust the numbers in your `.txt` files (enemy HP, move power, stat checks, XP rewards) until the experience feels challenging but fair.

**Verification Test:** Play the game twice and get different random encounters on the same path. Fulfill the condition for an achievement and see the notification pop up. The game's difficulty should feel smooth—no sudden, unfair difficulty spikes.

---

#### **Step 4.4: Shipping the Product (Final Build and Distribution)**

**Objective:** To package your game so that anyone can download and play it without needing to compile it themselves.

**Conceptual Plan:** You will compile a final, optimized version of your code and bundle it with the `assets` folder into a single, clean package.

**Actionable Steps:**
1.  **Compile the Executable:** Compile your code with optimizations enabled.
2.  **Create the Final Directory:** Create a new folder, `MemeLife_v1.0`.
3.  **Organize the Files:**
    *   Place your compiled executable (e.g., `MemeLife.exe` or `memelife`) inside the `MemeLife_v1.0` folder.
    *   Copy your entire `assets` folder into the `MemeLife_v1.0` folder. The executable and the `assets` folder must be in the same directory for the game to find its content.
4.  **Write the README:** Create a `README.txt` file. Explain what the game is, give credit to yourself, and provide simple instructions on how to run the executable.
5.  **Package It:** Compress the entire `MemeLife_v1.0` folder into a single `.zip` file.

**Verification Test:** On a different computer (or ask a friend), download the `.zip` file. Unzip it. Double-click the executable. The game must run perfectly without any extra setup or installation.

**Mission Accomplished.** If you complete this final test, you have successfully taken "MemeLife: Rise of the NPC" from a concept to a complete, distributable game. Well done.