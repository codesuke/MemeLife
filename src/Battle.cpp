#include "Battle.h"
#include <algorithm>

Boss DefaultBoss() {
    Boss b;
    b.name = "Karen Overlord";
    b.maxHp = 80; // Boss tougher than player
    b.hp = b.maxHp;
    return b;
}

int PlayerNormalAttack(BattleContext& ctx, Player& player) {
    (void)player; // not used yet; could scale with strength
    int dmg = 3; // fixed per spec
    ctx.boss.hp = std::max(0, ctx.boss.hp - dmg);
    ctx.playerAttackCount++;
    if (ctx.playerAttackCount >= 3) ctx.finisherAvailable = true;
    if (ctx.boss.hp == 0) { ctx.isOver = true; ctx.playerWon = true; }
    ctx.lastMessage = "You use Horrible Singing for " + std::to_string(dmg) + " damage.";
    return dmg;
}

int PlayerFinisher(BattleContext& ctx, Player& player) {
    (void)player; // could scale with memePower later
    if (!ctx.finisherAvailable) return 0;
    // Finisher ends the fight
    int dmg = ctx.boss.hp;
    ctx.boss.hp = 0;
    ctx.isOver = true; ctx.playerWon = true;
    ctx.lastMessage = " ENERGY BEAM obliterates the boss for " + std::to_string(dmg) + " damage!";
    return dmg;
}

int BossAttack(BattleContext& ctx, Player& player) {
    (void)ctx;
    int dmg = 12; // increased damage to make boss more threatening
    player.stats.hp = std::max(0, player.stats.hp - dmg);
    if (player.stats.hp == 0) { ctx.isOver = true; ctx.playerWon = false; }
    // More dramatic message when player is defeated
    if (player.stats.hp == 0) {
        ctx.lastMessage = "You have been WASTED! The boss dealt " + std::to_string(dmg) + " damage and defeated you.";
    } else {
        ctx.lastMessage = "Boss unleashes a Rick Roll wave for " + std::to_string(dmg) + " damage.";
    }
    return dmg;
}
