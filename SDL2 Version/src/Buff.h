#pragma once
#include <string>

struct Buff {
    std::string name;
    std::string description;
    
    // Stat modifiers (multiplicative and additive)
    float cloutModifier = 1.0f;
    float logicModifier = 1.0f;
    float sanityModifier = 1.0f;
    float moneyModifier = 1.0f;
    float energyModifier = 1.0f;
    float energyDrainModifier = 1.0f;  // Higher = drains faster
    
    // Additive bonuses applied after multipliers
    int cloutBonus = 0;
    int logicBonus = 0;
    int sanityBonus = 0;
    int moneyBonus = 0;
    float energyBonus = 0.0f;
};

class BuffDatabase {
public:
    static Buff GetBuffForState(const std::string& stateName);
    static const char* GetIndianStatesList();
    static int GetIndianStatesCount();
    
private:
    static constexpr const char* STATES[] = {
        "Andhra Pradesh",
        "Arunachal Pradesh",
        "Assam",
        "Bihar",
        "Chhattisgarh",
        "Goa",
        "Gujarat",
        "Haryana",
        "Himachal Pradesh",
        "Jharkhand",
        "Karnataka",
        "Kerala",
        "Madhya Pradesh",
        "Maharashtra",
        "Manipur",
        "Meghalaya",
        "Mizoram",
        "Nagaland",
        "Odisha",
        "Punjab",
        "Rajasthan",
        "Sikkim",
        "Tamil Nadu",
        "Telangana",
        "Tripura",
        "Uttar Pradesh",
        "Uttarakhand",
        "West Bengal"
    };
};
