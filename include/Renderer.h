#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <vector>
#include "GameData.h"

// Basic drawing helpers
void DrawBox(int x1, int y1, int x2, int y2);
void DrawText(int x, int y, const std::string& text);

// HUD and Screens
// topY lets us place HUD immediately under the map regardless of map height
void DrawHUD(const Player& player, int topY);
void DrawMenu();
void DrawPopup(const std::vector<std::string>& lines, const std::vector<std::string>& options);

// Map & Gameplay
void DrawMap(const std::vector<std::string>& grid, int playerX, int playerY);

// Quiz
struct QuizQuestion; // forward declare, defined in Quiz.h
void DrawQuiz(const QuizQuestion& q);

// Battle
struct BattleContext; // forward declare, defined in Battle.h
void DrawBattle(const BattleContext& ctx, const Player& player);

#endif // RENDERER_H
