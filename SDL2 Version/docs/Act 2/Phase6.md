Excellent. The world is built, connected, and populated. It feels like a real place. Now, in Phase 6, we give that world a heartbeat. This phase is about creating a "Day in the Life" experience. We are transforming the static world into a dynamic simulation where time matters and the player has clear goals.

As your Lead Designer, this is the blueprint for Phase 6. We are building the core gameplay loop.

---

### **Phase 6 Blueprint: The Core Loop**

**Your Goal for This Week:** To implement systems that make the world feel alive and give the player meaningful things to do. NPCs will no longer be static statues; they will follow simple daily schedules. You will introduce a basic quest system, allowing the player to take on tasks. You will create a simple economy with shops. Finally, the "sleep" mechanic will be fully implemented as the primary way to save progress and advance the game.

---

#### **Step 6.1: A World in Motion (NPC Scheduling System)**

**Objective:** To make NPCs move and exist in different locations based on the in-game time.

**Conceptual Plan:** Each NPC will have a simple schedule, which is a list of locations and corresponding times. For example: "Hostel Room (8 PM - 8 AM), Classroom (9 AM - 5 PM)." The `NPCManager` will be responsible for checking the in-game clock and updating the position of each NPC accordingly.

**Actionable Steps:**
1.  **Define a Schedule Data Structure:**
    *   Create a `ScheduleEntry` struct: `int startHour;`, `string mapId;`, `int x;`, `int y;`.
    *   In your `NPC` class, add a `vector<ScheduleEntry>` to hold its entire daily schedule.

2.  **Load Schedules:**
    *   You can define schedules in the NPC's data file or even in Tiled's object properties using a structured string (e.g., `"8,hostel.json,128,256;17,campus.json,512,320"`).
    *   Update your NPC loading logic to parse this data and populate the schedule vector for each NPC.

3.  **Create the `NPCManager`:**
    *   If you don't have one already, create a dedicated `NPCManager` class.
    *   This manager will hold a list of all NPCs currently active in the game world, regardless of which map they are on.
    *   Create a function `void NPCManager::Update(TimeManager& timeManager, string currentMapId)`.

4.  **Implement the Update Logic:**
    *   Inside `NPCManager::Update()`:
        1.  Loop through all your NPCs.
        2.  For each NPC, check its schedule against the `timeManager.currentHour`. Find the schedule entry that is currently active.
        3.  **Visibility Check:** Check if the NPC's active schedule entry places it on the `currentMapId` the player is on.
        4.  If the NPC should be on the current map, set its `isVisible` flag to `true` and update its `x, y` position to the coordinates from the schedule.
        5.  If the NPC should be on a different map, set its `isVisible` flag to `false`.
    *   Your main rendering loop will now only draw NPCs if their `isVisible` flag is true.

**Verification Test:** Place an NPC with a simple schedule (e.g., in the hostel room until 9 AM, then on the campus map after 9 AM). Start the game before 9 AM; you should see the NPC in the room. Wait for the in-game clock to pass 9 AM. The NPC should disappear. If you then walk to the campus map, you should see them at their new scheduled location.

---

#### **Step 6.2: Giving the Player Purpose (Basic Quest System)**

**Objective:** To create a system that can track tasks given to the player, allowing for simple "fetch quests" or objectives.

**Conceptual Plan:** We will create a `Quest` class and a `QuestManager`. A quest will have stages and objectives. An NPC can give the player a quest, which gets added to the player's quest log. The `QuestManager` will check if the conditions to advance or complete a quest have been met.

**Actionable Steps:**
1.  **Define the Quest Structure:**
    *   Create a `Quest` class. It needs:
        *   `string questId;`
        *   `string title;`
        *   `string description;`
        *   `int currentStage;`
        *   A data structure for its objectives (e.g., a `map<int, QuestObjective>`). An objective could be `TYPE_FETCH_ITEM`, `TYPE_TALK_TO_NPC`, etc.
        *   A boolean `isComplete;`.

2.  **Create the Quest Manager:**
    *   Create `QuestManager.h` and `QuestManager.cpp`.
    *   It will hold a `vector<Quest>` representing the player's active quests (their "quest log").
    *   **Functions:**
        *   `void StartQuest(string questId);`: Loads a quest definition from a file and adds it to the active list.
        *   `void AdvanceQuest(string questId);`: Increments a quest's `currentStage`.
        *   `bool IsQuestActive(string questId);`: Checks if a quest is in the log.
        *   `int GetQuestStage(string questId);`: Returns the current stage of a quest.

3.  **Integrate with the Event System:**
    *   This is the key step. Your `EventManager` needs new commands:
        *   `START_QUEST quest_id_001`: Calls `questManager.StartQuest()`.
        *   `ADVANCE_QUEST quest_id_001`: Calls `questManager.AdvanceQuest()`.
        *   `COMPLETE_QUEST quest_id_001`: Sets the quest's `isComplete` flag to true and gives the player a reward (XP, money, etc.).
    *   You also need a way to check quest status in dialogue. An event file might have conditional dialogue: `IF_QUEST_STAGE quest_id_001 1 DIALOGUE "Did you get the book yet?"`.

4.  **Create a Simple Quest UI:**
    *   Create a simple UI screen for the "Quest Log." When the player presses a key (e.g., 'J'), it should pause the game and display a list of their active quests and descriptions.

**Verification Test:** Create a Professor NPC. When you first talk to them, their event file triggers `START_QUEST "library_book"`. Your quest log now shows "Find the missing textbook." Create a "book" interactable object in the library map. Interacting with the book triggers `ADVANCE_QUEST "library_book"`. When you return to the professor, his dialogue is now different because the quest is on stage 2. He then triggers `COMPLETE_QUEST "library_book"` and gives you a reward.

---

#### **Step 6.3: Building the Economy (Shops & Items)**

**Objective:** To allow the player to spend their money on items that have a tangible gameplay effect.

**Conceptual Plan:** We will define a set of "Item" objects. Certain NPCs ("Shopkeepers") will have a special "shop" interaction that opens a UI menu, allowing the player to exchange money for these items. The player will have an inventory to store what they've bought.

**Actionable Steps:**
1.  **Define Items:**
    *   Create an `Item` class or struct: `string itemId;`, `string name;`, `string description;`, `int price;`, and an `Effect` struct (e.g., `effectType = RESTORE_ENERGY`, `effectValue = 25`).
    *   Create an "Item Database" that can load all possible item definitions from a file.

2.  **Create the Inventory System:**
    *   In your `Player` class, add a `vector<Item>` or a `map<string, int>` to act as their inventory.
    *   Create an "Inventory" UI screen, accessible by a key (e.g., 'I'). It should list the items the player owns and allow them to select and "use" an item.
    *   Using an item should apply its effect (e.g., `player.energy += 25`) and remove it from the inventory.

3.  **Build the Shop UI:**
    *   Design a new UI screen specifically for shopping. It should have two panels: the shop's inventory on the left and the player's inventory and money on the right.
    *   The player can navigate the shop's items, see their prices and descriptions, and press a "buy" button.

4.  **Implement Shopkeeper Logic:**
    *   Designate a specific NPC as a shopkeeper.
    *   Their interaction event will not trigger a normal dialogue. Instead, it will have a new command: `OPEN_SHOP shop_id_001`.
    *   The `EventManager`, upon seeing this command, will tell the `UIManager` to open the shop screen, loading the inventory specific to that shop ID.
    *   The shop UI will handle the logic for buying (check if `player.money >= item.price`) and selling.

**Verification Test:** You can walk up to a "Vending Machine" NPC. Interacting with it opens a shop menu. You can see "Coffee" for 20 rupees. You can buy the coffee; your money decreases, and "Coffee" appears in your inventory. You can then open your inventory screen and "use" the coffee, which restores some of your energy bar.

---

#### **Step 6.4: Ending the Day (Sleep & Save Mechanic)**

**Objective:** To fully implement the sleep mechanic as the core function for daily progression and saving the game.

**Conceptual Plan:** Interacting with the player's bed in their hostel room will no longer just be a simple dialogue prompt. It will now be the primary mechanism for advancing the game clock, restoring the player's stats, and writing their progress to a save file.

**Actionable Steps:**
1.  **Refine the Bed Interaction:**
    *   The event triggered by the player's bed (`EV_SLEEP_PROMPT`) should now present the player with a clear choice: "Sleep until morning? (This will save your game.)" with "Yes" and "No" options.

2.  **Implement the "Sleep" Functionality:**
    *   If the player chooses "Yes," a `GoToSleep()` function should be called. This function will:
        1.  Play a fade-to-black screen transition.
        2.  Call `timeManager.AdvanceDay()`.
        3.  Set the `timeManager.currentHour` to a fixed "wake up" time (e.g., 8 AM).
        4.  Fully restore the player's `energy` to `maxEnergy`.
        5.  **Trigger the Save Game function.**
        6.  Play a fade-in screen transition.

3.  **Create Robust Save/Load Systems:**
    *   Flesh out your save/load functions from the earlier planning stages.
    *   **What to Save:** You now need to save much more data:
        *   Player's stats, money, and inventory.
        *   Current in-game day and time.
        *   The state of all quests (which are active, their current stages).
        *   (Advanced) Any important world flags or persistent changes.
    *   **How to Save:** Serialize all this data into a structured format like JSON in your `.sav` file.
    *   Ensure your "Load Game" option on the main menu correctly parses this file and restores the entire game state.

**Final Verification Test for Phase 6:** You can play through a full in-game day. Your energy depletes. You complete the first stage of a quest. You buy a coffee. You return to your room, interact with the bed, and choose to sleep. The screen fades, the day advances to the next morning, and your energy is full. You can then quit the game, relaunch it, click "Load Game," and you will be exactly where you were, with the quest still active and the coffee still in your inventory. The core gameplay loop is now complete.