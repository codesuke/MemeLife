### **Phase 7 Blueprint: Weaving the Narrative**

**Your Goal for This Week:** To implement the game's first major story arc from beginning to end. You will enhance the event system to support choices and consequences. You will script at least one high-stakes "Canon Event." Finally, you will write a significant amount of ambient NPC dialogue to make the world feel reactive and alive.

---

#### **Step 7.1: The Main Questline (The "First Semester")**

**Objective:** To design and implement the central narrative quest that will guide the player through the initial part of the game.

**Conceptual Plan:** A good quest tells a story with a clear beginning, middle, and end. We will design a multi-stage quest based on a classic college experience: surviving the first mid-term exams. This provides a natural structure with clear objectives and a climactic finish.

**Actionable Steps:**
1.  **Outline the Quest on Paper:** Before writing any code or scripts, map out the story beats. This is a pure design step.
    *   **Quest ID:** `MQ01_MidTerms`
    *   **Title:** "Mid-Term Madness"
    *   **Stage 0 (Start):** The player attends a class where a professor announces the upcoming mid-terms, stressing their importance. The quest is automatically started.
    *   **Stage 1 (Objective):** "Find the Syllabus." The player needs to figure out where to get it—maybe by talking to a senior NPC or checking a notice board.
    *   **Stage 2 (Objective):** "Gather Notes." The player needs to acquire notes for a key subject. This could involve helping another student to get their notes, or buying them.
    *   **Stage 3 (Climax):** "Take the Exam." On a specific in-game date, the exam event is triggered. This will be a major stat check.
    *   **Stage 4 (Resolution):** "Check the Results." A few days later, the results are posted. The outcome (pass or fail) will have lasting consequences.

2.  **Create the Quest Definition File:**
    *   In an `assets/quests/` folder, create `MQ01_MidTerms.json` (or `.txt`).
    *   Define the quest's ID, title, and descriptions for each stage. This file tells the `QuestManager` what the quest is.

3.  **Implement the Quest Triggers:**
    *   Create the event file for the professor's initial lecture. The last command in this file should be `START_QUEST MQ01_MidTerms`.
    *   Script the events for the senior NPC, the notice board, etc., which will trigger the `ADVANCE_QUEST MQ01_MidTerms` command when the objective is met.

**Verification Test:** You can start the game, talk to the professor, and see the "Mid-Term Madness" quest appear in your quest log. You can complete the first objective (finding the syllabus), and the quest log updates to show the next objective.

---

#### **Step 7.2: Advanced Event Scripting (Choices & Consequences)**

**Objective:** To upgrade the `EventManager` to handle conditional logic, allowing events to have different outcomes based on the player's stats or quest progress.

**Conceptual Plan:** Our event system is currently linear. To tell a good story, it needs to be able to ask questions like "Is the player's Logic stat high enough?" or "Has the player started the mid-term quest yet?". We will add new conditional commands to our event scripting language.

**Actionable Steps:**
1.  **Design the Conditional Commands:**
    *   Define the new commands your event parser will need to understand:
        *   `CHECK_STAT [stat_name] [value]`: Checks if a player stat is greater than or equal to a value.
        *   `CHECK_QUEST_STAGE [quest_id] [stage_number]`: Checks if a quest is at a specific stage.
        *   `CHECK_HAS_ITEM [item_id]`: Checks if the player has an item in their inventory.
    *   Each of these commands needs two associated outcome commands: `SUCCESS_EVENT [event_id]` and `FAILURE_EVENT [event_id]`.

2.  **Update the Event Parser:**
    *   This is the main coding task. Refactor your `EventManager::TriggerEvent` function.
    *   When it reads a "CHECK" command, it must:
        1.  Perform the check against the player's current state.
        2.  Based on the true/false result, read the *next* line to find either the `SUCCESS_EVENT` or `FAILURE_EVENT` command.
        3.  Trigger the corresponding event ID, effectively creating a branching path.

3.  **Script a Branching Event:**
    *   Create an event for the "Take the Exam" climax of your main quest.
    *   The `EV_TakeTheExam.txt` file would look like this:
        ```
        DIALOGUE "The exam begins. You try to recall your notes..."
        CHECK_STAT Logic 25
        SUCCESS_EVENT EV_PassExam
        FAILURE_EVENT EV_FailExam
        ```
    *   Create the two outcome event files, `EV_PassExam.txt` and `EV_FailExam.txt`, each with different dialogue and consequences (like calling `ADVANCE_QUEST` to the "Pass" or "Fail" stage).

**Verification Test:** When the exam event triggers, if your Logic is high, you see a success message and the quest progresses. If you use a cheat or debug key to lower your Logic and trigger the event again, you see the failure message and a different quest outcome.

---

#### **Step 7.3: The "Canon Event" (A High-Stakes Moment)**

**Objective:** To implement one of the game's signature, high-risk narrative moments that can drastically alter the game or end it.

**Conceptual Plan:** A "Canon Event" is a specially designed quest or interaction that uses our advanced event scripting to present a difficult choice with severe consequences, including a potential Game Over. It should be a memorable, dramatic scenario.

**Actionable Steps:**
1.  **Design the Canon Event:** Let's use a classic example: "The Cheating Dilemma."
    *   **The Setup:** A shady NPC offers to sell the player "cheat notes" for the upcoming mid-term exam for a high price.
    *   **The Choice:** The player can buy the notes (`ADD_ITEM cheat_notes`) or refuse (`ADD_STAT logic 2`).
2.  **Script the Exam with a New Condition:**
    *   Modify the `EV_TakeTheExam.txt` event from the previous step. It now needs to check for the cheat notes *first*.
    *   The script would be:
        ```
        CHECK_HAS_ITEM cheat_notes
        SUCCESS_EVENT EV_AttemptToCheat
        FAILURE_EVENT EV_TakeExamNormally // This leads to our old Logic check
        ```
3.  **Script the Consequence:**
    *   Create the `EV_AttemptToCheat.txt` event. This introduces a new check—a "luck" or "risk" check.
        ```
        DIALOGUE "You discreetly pull out the cheat notes..."
        CHECK_STAT Sanity 40 // Low sanity means you're too nervous and get caught
        SUCCESS_EVENT EV_CheatSuccess
        FAILURE_EVENT EV_CaughtCheating
        ```
4.  **Implement the Ultimate Failure:**
    *   The `EV_CaughtCheating.txt` event is where the hammer falls:
        ```
        DIALOGUE "The professor spots you! You're dragged to the principal's office and expelled."
        GAME_OVER // A new event command
        ```
    *   Your `EventManager` must be able to call the main game's `GameOver()` function when it sees this command.

**Verification Test:** You can play through the scenario. If you buy the cheat notes and your Sanity is low, triggering the exam results in an immediate, narrative-driven Game Over screen. If your Sanity is high, you pass the exam easily.

---

#### **Step 7.4: Fleshing Out the World (Ambient Dialogue)**

**Objective:** To make the NPCs who *aren't* part of the main quest feel alive and aware of the world.

**Conceptual Plan:** Most NPCs shouldn't be silent statues. Their dialogue should change based on the time of day, where the player is in the main story, or other world events. This makes the world feel dynamic and is very efficient to implement with our new event system.

**Actionable Steps:**
1.  **Create Time-Based Dialogue:**
    *   Edit the event file for a generic student NPC. Use conditional checks to give them different lines for different times.
    *   `CHECK_TIME_BETWEEN 8 12` -> `SUCCESS_EVENT EV_MorningChat` (`DIALOGUE "Another lecture... I need coffee."`)
    *   `CHECK_TIME_BETWEEN 18 22` -> `SUCCESS_EVENT EV_EveningChat` (`DIALOGUE "Wanna go to the canteen?"`)

2.  **Create Quest-Aware Dialogue:**
    *   Make NPCs acknowledge the main quest.
    *   `CHECK_QUEST_STAGE MQ01_MidTerms 1` -> `SUCCESS_EVENT EV_MidtermGossip` (`DIALOGUE "Hey, did you find the syllabus yet?"`)
    *   This makes NPCs feel like they inhabit the same world as the player.

3.  **Write, Write, Write:**
    *   This is a content-heavy task. Your goal is to go through at least 5-10 of your generic, non-quest-critical NPCs and give each of them 2-3 different contextual lines of dialogue.

**Final Verification Test for Phase 7:** The game now has a clear story. You can play the entire "Mid-Term Madness" quest from start to its branching finish. The world feels more alive, as generic NPCs comment on the time of day and your quest progress. The game presents you with at least one high-stakes choice that could lead to a very bad (but entertaining) outcome.