#include "TimeManager.h"
#include "Player.h"
#include <iostream>

TimeManager::TimeManager()
    : currentDay(1), currentHour(8), currentMinute(0), minuteAccumulator(0.0f), 
      newHourThisFrame(false), newDayThisFrame(false) {
}

TimeManager::~TimeManager() {
}

void TimeManager::Update(float deltaTime) {
    // Reset flags
    newHourThisFrame = false;
    newDayThisFrame = false;
    
    // Convert real time to in-game time
    // SECONDS_PER_MINUTE = 0.5 means 0.5 real seconds = 1 in-game minute
    float inGameMinutes = deltaTime * (60.0f / SECONDS_PER_MINUTE);
    minuteAccumulator += inGameMinutes;
    
    // Handle minute rollover
    while (minuteAccumulator >= 1.0f) {
        currentMinute++;
        minuteAccumulator -= 1.0f;
        
        // Handle hour rollover
        if (currentMinute >= MINUTES_PER_HOUR) {
            currentHour++;
            currentMinute -= MINUTES_PER_HOUR;
            newHourThisFrame = true;  // Flag that a new hour has passed
            
            // Handle day rollover
            if (currentHour >= HOURS_PER_DAY) {
                AdvanceDay();
                currentHour -= HOURS_PER_DAY;
                newDayThisFrame = true;
            }
        }
    }
}

void TimeManager::AdvanceDay(Player* player) {
    currentDay++;
    
    // Check for level up
    if (player && (currentDay % DAYS_PER_LEVEL == 0)) {
        std::cout << "Day " << currentDay << " reached! Level up condition triggered." << std::endl;
        player->LevelUp();
    }
}

float TimeManager::GetTotalSeconds() const {
    return (currentDay - 1) * HOURS_PER_DAY * MINUTES_PER_HOUR * SECONDS_PER_MINUTE +
           currentHour * MINUTES_PER_HOUR * SECONDS_PER_MINUTE +
           currentMinute * SECONDS_PER_MINUTE;
}
