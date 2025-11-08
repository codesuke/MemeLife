Right. With the engine established, it's time to bring the world to life. Phase 2 is about making the game world interactive and introducing the fundamental simulation elements.

As your Lead Designer, this is your detailed, no-code blueprint for Phase 2. We're focusing on the UI, the passage of time, and the player's ability to trigger simple events.

---

### **Phase 2 Blueprint: The Interactive Layer**

**Your Goal for This Week:** To implement a functional UI system capable of displaying dialogue boxes with text that types out over time. You will create an in-game clock that tracks days and time. Finally, you will create an interaction system allowing the player to press a key near a specific object to trigger a pre-written dialogue event.

---

#### **Step 2.1: The Game Clock & Time System**

**Objective:** To create a system that tracks the in-game time, progressing from morning to night and counting the days. This is the central pillar of the game's progression and simulation.

**Conceptual Plan:** We need a central manager for time. This manager will keep track of days, hours, and minutes. As the real-world game loop runs, this manager will tick forward the in-game clock. We'll then display this time on the screen and use it to potentially change the game's atmosphere.

**Actionable Steps:**
1.  **Create the Time Manager:**
    *   Create `TimeManager.h` and `TimeManager.cpp`.
    *   **Data:** Declare private member variables: `int currentDay`, `int currentHour`, `int currentMinute`.
    *   **Constants:** Define constants for how much real-world time corresponds to in-game time: `const float SECONDS_PER_MINUTE = 0.5f;` (This means 0.5 real seconds for 1 in-game minute – adjust this to taste for speed). `const int MINUTES_PER_HOUR = 60;`, `const int HOURS_PER_DAY = 24;`.
    *   **Functions:** Declare public functions: `void Update(float deltaTime)`, `void Render(Renderer& renderer)`, `void AdvanceDay()`.

2.  **Implement Time Progression:**
    *   In `TimeManager::Update(float deltaTime)`:
        *   Add `deltaTime * (60.0f / SECONDS_PER_MINUTE)` to `currentMinute`. This scales real time into in-game minutes.
        *   **Handle Minute Rollover:** If `currentMinute >= MINUTES_PER_HOUR`:
            *   Increment `currentHour`.
            *   Set `currentMinute -= MINUTES_PER_HOUR`.
        *   **Handle Hour Rollover:** If `currentHour >= HOURS_PER_DAY`:
            *   Call `AdvanceDay()`.
            *   Set `currentHour -= HOURS_PER_DAY`.

3.  **Implement Day Advancement:**
    *   In `TimeManager::AdvanceDay()`:
        *   Increment `currentDay`.
        *   (Later, this will be where save game logic or daily resets happen).
        *   For now, it's just a counter.

4.  **Display the Time:**
    *   In your `Renderer.cpp`, add a function `void RenderTime(const TimeManager& timeManager, Font& font)`. This function will take the `TimeManager` and your loaded font.
    *   It will format the time into a string (e.g., "Day: 3 - 14:30").
    *   Use your existing `RenderText` functionality to draw this string to a specific position on the screen (e.g., top-right corner).
    *   Modify your main `Renderer::RenderUI()` function to call this new `RenderTime` function.

5.  **(Stretch Goal) Day/Night Visual Tint:**
    *   In `Renderer::RenderUI()`, after clearing the screen but before rendering other elements, check the `TimeManager::currentHour`.
    *   Based on the hour, set a subtle tint. You'll need to use `SDL_SetRenderDrawBlendMode(renderer, SDL_BLEND_RGBA_MULT);` (or `SDL_BLEND_ALPHA_ADD`) and then `SDL_SetRenderDrawColor(renderer, r, g, b, a);` followed by `SDL_RenderFillRect(renderer, nullptr);` (to draw a rectangle over the whole screen).
        *   **Day (approx. 6 AM - 6 PM):** Normal color, no tint.
        *   **Evening (approx. 6 PM - 9 PM):** Faint orange tint.
        *   **Night (approx. 9 PM - 6 AM):** Dark blue/purple tint.

**Verification Test:** Run the game. A clock is visible and ticks forward. You can observe the minutes, hours, and days changing. If you implemented the stretch goal, the screen's color should visibly change to simulate time of day.

---

#### **Step 2.2: The UI & Dialogue Box System**

**Objective:** To create a system that can display text messages to the player, including a smooth "typewriter" effect, and a box to contain it.

**Conceptual Plan:** We need a visual container for text messages and a way to make that text appear gradually, as if being typed. A `DialogueBox` class will handle the appearance and the typing effect. A `UIManager` will manage whether a dialogue box is currently active.

**Actionable Steps:**
1.  **Create UI Assets:**
    *   In your `assets/ui/` folder, create `dialogue_box_bg.png`. This will be the background texture for your dialogue box. It should be designed to be scalable (using 9-slicing principles, but for simplicity, even a simple box graphic will work for now).
    *   Create `prompt_arrow.png`. A small sprite (like a triangle) to indicate the player can advance the text.
2.  **Design the Dialogue Box Class:**
    *   Create `DialogueBox.h` and `DialogueBox.cpp`.
    *   **Data:** `string fullText`, `string visibleText`, `float typewriterTimer`, `float timePerCharacter`, `SDL_Rect position`, `SDL_Texture* backgroundTexture`, `SDL_Texture* promptArrowTexture`.
    *   **Functions:** `DialogueBox(string text, Font& font, Renderer& renderer)`, `void Update(float deltaTime)`, `void Render(Renderer& renderer, Font& font)`.

3.  **Implement Typewriter Effect:**
    *   In `DialogueBox::Update(float deltaTime)`:
        *   If `visibleText.length() < fullText.length()`:
            *   Add `deltaTime` to `typewriterTimer`.
            *   If `typewriterTimer >= timePerCharacter`:
                *   Append the next character from `fullText` to `visibleText`.
                *   Reset `typewriterTimer = 0;`.
    *   This `timePerCharacter` controls the typing speed.

4.  **Implement Rendering:**
    *   In `DialogueBox::Render()`:
        *   Load `dialogue_box_bg.png` into `backgroundTexture` and `prompt_arrow.png` into `promptArrowTexture` in the constructor.
        *   Draw the `backgroundTexture` at the `position`.
        *   Render `visibleText` using your `Font` object and `Renderer::RenderText` function.
        *   If `visibleText.length() == fullText.length()` (text is fully typed), render the `promptArrowTexture` at the bottom corner of the dialogue box.

5.  **Create the UIManager:**
    *   Create `UIManager.h` and `UIManager.cpp`.
    *   **Data:** A pointer to the currently active `DialogueBox* currentDialogue`. A reference to the `Font` object and `Renderer`.
    *   **Functions:** `UIManager(Font& font, Renderer& renderer)`, `void ShowDialogue(string text)`, `void Update(float deltaTime)`, `void Render()`.
    *   `ShowDialogue()` will create a new `DialogueBox`, set `currentDialogue` to point to it, and load the text and necessary textures.
    *   `Update()` will call `currentDialogue->Update()` if it exists.
    *   `Render()` will call `currentDialogue->Render()` if it exists.

**Verification Test:** In your main game loop's `Update` and `Render` functions (outside the `TimeManager` calls), add logic: when the game is in a "dialogue" state, call `uiManager.Update()` and `uiManager.Render()`. For testing, hardcode a call in your main setup to `uiManager.ShowDialogue("This is a test message for the dialogue box! It will appear character by character.")`. You should see the box and the text typing out.

---

#### **Step 2.3: The Interaction System**

**Objective:** To allow the player character to interact with specific objects in the game world, triggering the dialogue system.

**Conceptual Plan:** We will mark certain tiles or objects on our map as "interactable." The player will move their character on the map. When they are in a specific proximity to an interactable object and press a designated "interact" key, the game will trigger an event associated with that object.

**Actionable Steps:**
1.  **Define Interactable Objects:**
    *   Create a simple `Interactable` struct: `string id`, `SDL_Rect triggerArea`, `string eventIdToTrigger`.
    *   Create a list or vector of `Interactable` objects in your `Map` class or a new `ObjectManager` class.
    *   **For Testing:** Manually add one or two `Interactable` objects to your map. E.g., a "Bed" in the player's room with `triggerArea` defining its space, and `eventIdToTrigger = "EV_SLEEP_PROMPT"`.

2.  **Determine Player's Facing Tile/Area:**
    *   In your `Player` class, add logic to determine the player's "interaction range" or the tile they are facing. This could be a small `SDL_Rect` in front of the player.
    *   You'll need to know the player's current direction of movement for this.

3.  **Implement Interaction Check:**
    *   In your main game loop's `handleInput` function:
        *   Listen for a specific interaction key (e.g., 'E').
        *   If the key is pressed:
            1.  Get the player's current interaction area.
            2.  Iterate through your list of `Interactable` objects.
            3.  Check if the player's interaction area *overlaps* with any `Interactable`'s `triggerArea`.
            4.  If an overlap is found, get the `eventIdToTrigger` from that `Interactable`.

4.  **Trigger the Event via Dialogue:**
    *   Create a simple `EventManager` or a series of functions. For this phase, it can be a simple `switch` statement within your `handleInput` or `Game` update logic.
    *   When an `eventIdToTrigger` is obtained:
        *   `if (eventId == "EV_SLEEP_PROMPT")`: Call `uiManager.ShowDialogue("Do you want to go to sleep? It will advance to the next day.");`
        *   `if (eventId == "EV_POSTER_EXAMINE")`: Call `uiManager.ShowDialogue("It's a poster of a meme you barely remember. It's surprisingly dusty.");`
        *   **(Important):** You will need a way to handle the dialogue's own input. If the player presses 'E' while a dialogue box is active, it should advance the text or close the box. This input handling needs to check if a dialogue box is active *before* checking for general world interaction.

**Verification Test:** You can walk your player character next to the "Bed" or "Poster" object you placed. Pressing 'E' should trigger the dialogue box, displaying the associated text. You should be able to advance the text and close the dialogue box by pressing 'E' again.

---

With Phase 2 complete, you will have a tangible game world with a clock, interactive elements, and a basic dialogue system. The player can now explore and engage with the environment in a meaningful way.