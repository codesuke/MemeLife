#ifndef BATTLE_H
#define BATTLE_H

#include <string>
#include "GameData.h"

struct Boss {
    std::string name;
    int hp;
    int maxHp;
};

struct BattleContext {
    Boss boss;
    int playerAttackCount = 0;
    bool finisherAvailable = false;
    bool isOver = false;
    bool playerWon = false;
    std::string lastMessage;
};

// Initialize a default boss tuned so finisher is required
Boss DefaultBoss();

// Actions
int PlayerNormalAttack(BattleContext& ctx, Player& player); // returns damage dealt
int PlayerFinisher(BattleContext& ctx, Player& player);     // returns damage dealt
int BossAttack(BattleContext& ctx, Player& player);         // returns damage dealt

#endif // BATTLE_H
