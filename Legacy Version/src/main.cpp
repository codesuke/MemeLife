// Phase 2 prototype main with state machine for Level 1 map, quiz, battle, save
#include <iostream>
#include <vector>
#include <string>
#include "GameData.h"
#include "Terminal.h"
#include "Renderer.h"
#include "Map.h"
#include "Quiz.h"
#include "Battle.h"
#include "Save.h"

using namespace std;

enum class GameState { MENU, MAP, QUIZ, BATTLE, POST_LEVEL, EXIT };

struct Game {
    Player player;
    Map map;
    QuizBank quizBank;
    GameState state = GameState::MENU;
    QuizQuestion activeQuestion; // current quiz
    BattleContext battleCtx;
    bool bossTriggered = false;
    string savePath = "memelife.sav";

    Game() {
        // Initialize player baseline stats per spec (lower hp than boss)
        player.name = "Player";
        player.memeType = "Novice";
        player.level = 1;
        player.xp = 0;
        player.stats.clout = 20;
        player.stats.logic = 20;
        player.stats.sanity = 20;
        player.stats.hp = 60;      // lower than boss
        player.stats.maxHp = 60;
        player.stats.strength = 10;
        player.stats.memePower = 10;
        map.LoadDefault();
        quizBank.LoadDefault();
    }

    void NewGame() {
        map.LoadDefault();
        map.RandomizeEvents(3, false); // randomize only E tiles; keep boss at layout position
        bossTriggered = false;
        state = GameState::MAP;
    }

    void LoadGame() {
        // Use global LoadGame from Save.cpp
        if (::LoadGame(savePath, player, map)) {
            state = GameState::MAP;
        } else {
            // fallback new game if load fails
            NewGame();
        }
    }

    void Save() {
        SaveGame(savePath, player, map);
    }

    void LevelUpAfterBoss() {
        player.level += 1;
        player.stats.maxHp += 10;
        player.stats.hp = player.stats.maxHp; // heal
        player.stats.memePower += 10;
        player.memeType = "Level" + to_string(player.level);
    }

    void Run() {
        bool running = true;
        while (running) {
            switch (state) {
                case GameState::MENU: {
                    DrawMenu();
                    char c = GetImmediateInput();
                    if (c == '1') NewGame();
                    else if (c == '2') LoadGame();
                    else if (c == 'q' || c == 'Q') { state = GameState::EXIT; }
                    break; }
                case GameState::MAP: {
                    ClearScreen();
                    DrawMap(map.grid, map.playerX, map.playerY);
                    // HUD right below the map: topY = map rows + 2
                    DrawHUD(player, (int)map.grid.size() + 2);
                    char c = GetImmediateInput();
                    // Normalize to lowercase to support uppercase input
                    if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
                    if (c == 'w') map.TryMove(0, -1);
                    else if (c == 's') map.TryMove(0, 1);
                    else if (c == 'a') map.TryMove(-1, 0);
                    else if (c == 'd') map.TryMove(1, 0);
                    else if (c == 'q') { state = GameState::MENU; break; }

                    char tile = map.GetTile(map.playerX, map.playerY);
                    if (tile == 'E') {
                        activeQuestion = quizBank.GetRandom();
                        state = GameState::QUIZ;
                    } else if (tile == 'B' && !bossTriggered) {
                        bossTriggered = true;
                        battleCtx.boss = DefaultBoss();
                        battleCtx.playerAttackCount = 0;
                        battleCtx.finisherAvailable = false;
                        battleCtx.isOver = false;
                        battleCtx.playerWon = false;
                        state = GameState::BATTLE;
                    }
                    break; }
                case GameState::QUIZ: {
                    DrawQuiz(activeQuestion);
                    char c = GetImmediateInput();
                    if (c == 'q') { state = GameState::MAP; break; }
                    if (c >= '1' && c <= '9') {
                        int idx = c - '1';
                        bool correct = (idx == activeQuestion.correctIndex);
                        // Provide clearer feedback and stronger penalty
                        if (correct) {
                            player.stats.strength += 5;
                            // Inform the player which stat increased
                            ClearScreen();
                            DrawPopup({"Correct!", "Strength increased by 5."}, {"OK"});
                            GetImmediateInput();
                        } else {
                            int penalty = 10; // larger HP penalty for wrong answers
                            player.stats.hp -= penalty;
                            if (player.stats.hp < 0) player.stats.hp = 0;
                            ClearScreen();
                            DrawPopup({"Incorrect!", string("HP reduced by ") + to_string(penalty) + " due to wrong answer."}, {"OK"});
                            GetImmediateInput();
                        }
                        // Consume the event tile so it can't trigger again
                        map.SetTile(map.playerX, map.playerY, '^');
                        // If player died due to wrong answer, go to menu
                        if (player.stats.hp == 0) {
                            ClearScreen();
                            DrawPopup({"You have no HP left.", "Returning to Menu."}, {"OK"});
                            GetImmediateInput();
                            state = GameState::MENU;
                        } else {
                            state = GameState::MAP;
                        }
                    }
                    break; }
                case GameState::BATTLE: {
                    DrawBattle(battleCtx, player);
                    char c = GetImmediateInput();
                    if (c == '1') {
                        PlayerNormalAttack(battleCtx, player);
                        if (!battleCtx.isOver) BossAttack(battleCtx, player);
                    } else if (c == '2' && battleCtx.finisherAvailable) {
                        PlayerFinisher(battleCtx, player);
                    } else if (c == 'q') {
                        battleCtx.isOver = true; battleCtx.playerWon = false;
                    }
                    if (battleCtx.isOver) {
                        if (battleCtx.playerWon) {
                            LevelUpAfterBoss();
                            state = GameState::POST_LEVEL;
                        } else {
                            // Show defeat popup before returning to menu
                            ClearScreen();
                            DrawPopup({"You were defeated!", "WASTED!"}, {"OK"});
                            GetImmediateInput();
                            state = GameState::MENU; // defeat
                        }
                    }
                    break; }
                case GameState::POST_LEVEL: {
                    vector<string> lines = {"Boss Defeated!", "You leveled up."};
                    vector<string> opts = {"Enter Level 2 (placeholder)", "Save Game", "Exit to Menu"};
                    ClearScreen();
                    DrawPopup(lines, opts);
                    char c = GetImmediateInput();
                    if (c == '1') {
                        // Placeholder for Level 2 start
                        NewGame();
                    } else if (c == '2') {
                        Save();
                    } else if (c == '3') {
                        state = GameState::MENU;
                    }
                    break; }
                case GameState::EXIT: running = false; break;
            }
        }
        ClearScreen();
        cout << "Thanks for playing MemeLife Level 1 Prototype!" << endl;
    }
};

int main() {
    Game game;
    game.Run();
    return 0;
}
