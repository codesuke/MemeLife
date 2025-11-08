#include "Buff.h"

Buff BuffDatabase::GetBuffForState(const std::string& stateName) {
    if (stateName == "Delhi") {
        return {
            "Born Negotiator",
            "You grew up in Delhi, surrounded by power brokers.\n"
            "Clout +20%, Energy Drain -10%",
            1.2f,  // cloutModifier
            1.0f,  // logicModifier
            1.0f,  // sanityModifier
            1.0f,  // moneyModifier
            1.0f,  // energyModifier
            0.9f,  // energyDrainModifier
            0, 0, 0, 0, 0.0f  // bonuses
        };
    }
    else if (stateName == "Bangalore") {
        return {
            "Tech Savant",
            "You learned to code before you learned to walk.\n"
            "Logic +20%, Money +15%",
            1.0f,  // cloutModifier
            1.2f,  // logicModifier
            1.0f,  // sanityModifier
            1.15f, // moneyModifier
            1.0f,  // energyModifier
            1.0f,  // energyDrainModifier
            0, 0, 0, 0, 0.0f
        };
    }
    else if (stateName == "Mumbai") {
        return {
            "City of Dreams",
            "You learned hustle on the streets of Mumbai.\n"
            "Money +25%, Sanity -10%",
            1.0f,  // cloutModifier
            1.0f,  // logicModifier
            0.9f,  // sanityModifier
            1.25f, // moneyModifier
            1.0f,  // energyModifier
            1.0f,  // energyDrainModifier
            0, 0, 0, 0, 0.0f
        };
    }
    else if (stateName == "Kerala") {
        return {
            "Zen Mind",
            "The land of backwaters taught you peace.\n"
            "Sanity +15%, Energy +10%",
            1.0f,  // cloutModifier
            1.0f,  // logicModifier
            1.15f, // sanityModifier
            1.0f,  // moneyModifier
            1.1f,  // energyModifier
            1.0f,  // energyDrainModifier
            0, 0, 0, 0, 0.0f
        };
    }
    else if (stateName == "Punjab") {
        return {
            "Mighty Heart",
            "You're built tough in the land of five rivers.\n"
            "Clout +15%, Sanity +10%, Energy +5%",
            1.15f, // cloutModifier
            1.0f,  // logicModifier
            1.1f,  // sanityModifier
            1.0f,  // moneyModifier
            1.05f, // energyModifier
            1.0f,  // energyDrainModifier
            0, 0, 0, 0, 0.0f
        };
    }
    else if (stateName == "Rajasthan") {
        return {
            "Desert Warrior",
            "You learned to survive in the harsh desert.\n"
            "Energy +20%, Logic -10%",
            1.0f,  // cloutModifier
            0.9f,  // logicModifier
            1.0f,  // sanityModifier
            1.0f,  // moneyModifier
            1.2f,  // energyModifier
            1.0f,  // energyDrainModifier
            0, 0, 0, 0, 0.0f
        };
    }
    
    // Default buff for any other state
    return {
        "Balanced",
        "You're a well-rounded individual.",
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0, 0, 0, 0, 0.0f
    };
}

const char* BuffDatabase::GetIndianStatesList() {
    return "Andhra Pradesh, Arunachal Pradesh, Assam, Bihar, Chhattisgarh, Goa, Gujarat, Haryana, "
           "Himachal Pradesh, Jharkhand, Karnataka, Kerala, Madhya Pradesh, Maharashtra, Manipur, "
           "Meghalaya, Mizoram, Nagaland, Odisha, Punjab, Rajasthan, Sikkim, Tamil Nadu, Telangana, "
           "Tripura, Uttar Pradesh, Uttarakhand, West Bengal";
}

int BuffDatabase::GetIndianStatesCount() {
    return 28;
}
