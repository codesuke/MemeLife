#pragma once

class Player;

class TimeManager {
public:
    TimeManager();
    ~TimeManager();

    void Update(float deltaTime);
    bool IsNewHour() const { return newHourThisFrame; }  // Returns true if hour just passed
    bool IsNewDay() const { return newDayThisFrame; }    // Returns true if day just passed
    void AdvanceDay(Player* player = nullptr);           // Optional player for level up
    
    // Getters
    int GetDay() const { return currentDay; }
    int GetHour() const { return currentHour; }
    int GetMinute() const { return currentMinute; }
    
    float GetTotalSeconds() const;
    
    // Time constants
    static constexpr float SECONDS_PER_MINUTE = 0.5f;  // 0.5 real seconds = 1 in-game minute
    static constexpr int MINUTES_PER_HOUR = 60;
    static constexpr int HOURS_PER_DAY = 24;
    static constexpr int DAYS_PER_LEVEL = 30;           // Level up every 30 days

private:
    int currentDay;
    int currentHour;
    int currentMinute;
    float minuteAccumulator;  // For fractional minute tracking
    bool newHourThisFrame;    // Tracks if hour just passed
    bool newDayThisFrame;     // Tracks if day just passed
};


