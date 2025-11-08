### **Phase 8 Blueprint: Adding the "Meme" to MemeLife**

**Your Goal for This Week:** To fully integrate the game's signature humor and presentation. You will implement the complete sound effect library, making events feel reactive and funny. You will add visual gags and Easter eggs to the world. If time permits, you will prototype a simple "meme battle" system. Finally, and most importantly, you will conduct the first major balance pass to ensure the game is fun and fair to play.

---

#### **Step 8.1: The Sound of Memes (Sound Effect Integration)**

**Objective:** To make the game audibly hilarious by tying your library of meme sound effects directly into the event system.

**Conceptual Plan:** Our `EventManager` can already trigger dialogue and stat changes. We will expand its vocabulary so it can easily command the `AudioManager` to play specific, short sound effects at precise moments. This is the single most important step for establishing the game's comedic tone.

**Actionable Steps:**
1.  **Create the Sound Effect Manager:**
    *   Flesh out your `AudioManager`. It needs a clear function: `void PlaySfx(string filename)`.
    *   This function will be responsible for loading a sound effect (`.wav` or `.ogg`) from the `assets/sfx/` folder into memory, playing it once, and then releasing it. It's crucial to manage memory here so you don't load every sound effect at the start.

2.  **Upgrade the Event System with a Sound Command:**
    *   Add a new, simple command to your event scripting language: `PLAY_SFX [filename.wav]`.
    *   In your `EventManager::TriggerEvent` parser, when it sees this command, it should immediately call `audioManager.PlaySfx(argument)`.

3.  **Script the Sounds:**
    *   This is a creative, not a coding, task. Go back through all the event files you've written, especially for the main quest.
    *   Identify key moments and punctuate them with sound effects.
        *   **Player fails a Logic check?**
            ```
            DIALOGUE "Your argument makes no sense."
            PLAY_SFX bruh_moment.wav
            ```
        *   **Player passes a hard stat check?**
            ```
            DIALOGUE "You aced the exam!"
            PLAY_SFX wow_effect.wav
            ```
        *   **A dramatic reveal in the story?**
            ```
            DIALOGUE "It turns out... the professor was the one who stole the notes."
            PLAY_SFX vine_boom.wav
            ```
        *   **Opening the shop menu?** `PLAY_SFX cash_register.wav`.
        *   **Sleeping?** `PLAY_SFX snoring.wav`.

4.  **Refine Audio Channels:**
    *   Ensure your `AudioManager` plays music on one channel and sound effects on another. This allows a sound effect to play without interrupting the background music. SDL_mixer handles this naturally if you use `Mix_PlayMusic` for themes and `Mix_PlayChannel` for sound effects (`Mix_Chunk`).

**Verification Test:** Playing through the "Mid-Term Madness" quest is now a completely different experience. Actions and dialogue are punctuated by timely, humorous sound effects that provide instant, funny feedback.

---

#### **Step 8.2: A World of Jokes (Visual Gags & Easter Eggs)**

**Objective:** To reward observant players and make the world itself part of the humor.

**Conceptual Plan:** We will use our existing `Interactable` object system to add small, hidden jokes and references throughout the game world. These are low-effort, high-reward additions that make the environment feel handcrafted and clever.

**Actionable Steps:**
1.  **Brainstorm Hidden Jokes:** Think of funny, context-specific things to hide in your maps.
    *   A trash can in the hostel: Interacting with it gives the dialogue, "It's full of instant noodle cups. The lifeblood of an engineer."
    *   A specific book on a library shelf: "It's a textbook on ancient memeology. A classic."
    *   A crack in a wall: "You peek through the crack. You see your neighbor doing a TikTok dance. You quietly back away."
2.  **Implement via the Event System:**
    *   Go into Tiled Map Editor. In your `Objects` layer, place new, small, invisible rectangle objects over these features.
    *   Assign a new `eventId` to each one (e.g., `EV_TRASH_CAN_JOKE`, `EV_MEME_BOOK_JOKE`).
    *   Create the corresponding simple event files, which contain only a `DIALOGUE` command and maybe a `PLAY_SFX` command.
3.  **Create a Rare "Easter Egg" NPC:**
    *   Using your NPC scheduling system, create a new, unique NPC.
    *   Give them a schedule that only makes them appear in a specific, obscure location at a very specific time (e.g., in a corner of the campus map at 3:00 AM).
    *   Interacting with this NPC could give a rare item or a cryptic, funny message.

**Verification Test:** You can now explore the world and find at least 5-10 hidden jokes by interacting with background objects. The world feels less like a game level and more like a comedic space.

---

#### **Step 8.3: Prototyping "Meme Battles" (Optional Stretch Goal)**

**Objective:** To design and prototype a simple turn-based combat system for key narrative encounters.

**Conceptual Plan:** This is an ambitious but potentially high-impact feature. We would create a new game state, `BATTLE`, with its own UI and logic. It would be a simplified version of a classic RPG battle, but with moves based on our core stats.

**Actionable Steps:**
1.  **Define Battle Moves:** A move could be: `"Logic Bomb" (Deals damage based on player's Logic)` or `"Ratio" (Lowers enemy's Clout)`.
2.  **Design the Battle UI:** Create a new UI layout that shows the player's sprite, an enemy sprite, and a menu with 3-4 move options.
3.  **Create a `BattleManager`:** This class would manage the turn order, process move selections, calculate "damage" (to Sanity, Clout, or Logic), and check for a win/loss condition.
4.  **Integrate with the Event System:** Create a new event command, `START_BATTLE [enemy_id]`. This command would switch the game state to `BATTLE`. The outcome of the battle (win or lose) would then trigger a `SUCCESS_EVENT` or `FAILURE_EVENT`.

**Verification Test:** You can create an event that, when triggered, fades the screen to a new battle UI. You can select a move from a menu, which plays an animation and reduces the enemy's stat bar. The enemy then performs an action. This would confirm the core battle loop is functional.

---

#### **Step 8.4: The First Balance Pass**

**Objective:** To ensure the gameplay slice is fun, fair, and the player's choices feel meaningful.

**Conceptual Plan:** Now that all systems are integrated, the initial numbers we chose (prices, rewards, stat checks) are almost certainly wrong. We must play the game as a player would and adjust these values until the experience feels right.

**Actionable Steps:**
1.  **Play Through the Core Loop:** Play your "Mid-Term Madness" quest from start to finish multiple times, making different choices.
2.  **Ask Critical Questions and Adjust Values:**
    *   **Pacing:** Does the in-game clock move too fast or too slow? Adjust the `SECONDS_PER_MINUTE` constant.
    *   **Economy:** Can you afford to buy items? Is money too easy or too hard to get? Adjust shop prices and quest rewards.
    *   **Energy:** Are you constantly running out of energy? Is it never a problem? Adjust the `ENERGY_DRAIN_PER_HOUR` and the amount restored by items/sleep.
    *   **Difficulty:** Is the Logic check for the exam impossible to pass? Is it too easy? Adjust the value in the `CHECK_STAT` command.
    *   **Meaningful Choice:** Does the "cheat notes" path feel like a valid, tempting option? If not, adjust the price or the risk associated with it.
3.  **Centralize Constants:** Move all these magic numbers (`ENERGY_DRAIN_PER_HOUR`, `DAYS_PER_LEVEL`, etc.) to a single `Config.h` file. This makes them incredibly easy to find and tweak during balancing.

**Final Verification Test for Phase 8:** You can give the game to a friend. They can play the "gameplay slice" and understand what to do without your help. They find it funny. The difficulty feels challenging but not unfair. The core experience is solid, fun, and ready to be expanded with more content.

With the completion of Phase 8, Act II is finished. You have a playable, polished, and genuinely entertaining vertical slice of your game. You are now perfectly positioned to enter Act III: expanding this solid foundation into a full-fledged experience.