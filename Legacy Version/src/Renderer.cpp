#include "Renderer.h"
#include "Terminal.h"
#include "Quiz.h"
#include "Battle.h"
#include <iostream>
#include <algorithm>
#include <sstream>

using namespace std;

void DrawBox(int x1, int y1, int x2, int y2) {
    // Clamp and swap if needed
    if (x2 < x1) swap(x1, x2);
    if (y2 < y1) swap(y1, y2);
    // Corners
    MoveCursor(x1, y1); cout << "+";
    MoveCursor(x2, y1); cout << "+";
    MoveCursor(x1, y2); cout << "+";
    MoveCursor(x2, y2); cout << "+";
    // Horizontal
    for (int x = x1 + 1; x < x2; ++x) {
        MoveCursor(x, y1); cout << "-";
        MoveCursor(x, y2); cout << "-";
    }
    // Vertical
    for (int y = y1 + 1; y < y2; ++y) {
        MoveCursor(x1, y); cout << "|";
        MoveCursor(x2, y); cout << "|";
    }
}

void DrawText(int x, int y, const string& text) {
    MoveCursor(x, y);
    cout << text;
}

void DrawHUD(const Player& player, int top) {
    // Draw a HUD box at specified top row (under the map)
    int left = 2;
    int right = 78;
    int bottom = top + 5;
    DrawBox(left, top, right, bottom);
    int y = top + 1;
    DrawText(left + 2, y++, "Name: " + player.name + "  Type: " + player.memeType + "  Lv: " + to_string(player.level));
    DrawText(left + 2, y++, "HP: " + to_string(player.stats.hp) + "/" + to_string(player.stats.maxHp));
    DrawText(left + 2, y++, "Clout:" + to_string(player.stats.clout) + "  Logic:" + to_string(player.stats.logic) + "  Sanity:" + to_string(player.stats.sanity));
    DrawText(left + 2, y++, "Strength:" + to_string(player.stats.strength) + "  MemePower:" + to_string(player.stats.memePower));
}

void DrawMenu() {
    ClearScreen();
    DrawBox(10, 5, 70, 18);
    DrawText(20, 7, "MemeLife - Level 1");
    DrawText(20, 9, "1) New Game");
    DrawText(20, 10, "2) Load Game");
    DrawText(20, 11, "q) Quit");
}

void DrawPopup(const vector<string>& lines, const vector<string>& options) {
    int left = 10, top = 6, right = 70, bottom = 20;
    DrawBox(left, top, right, bottom);
    int y = top + 1;
    for (const auto& l : lines) {
        DrawText(left + 2, y++, l);
    }
    y++;
    for (size_t i = 0; i < options.size(); ++i) {
        DrawText(left + 2, y++, to_string(i + 1) + ") " + options[i]);
    }
}

void DrawMap(const vector<string>& grid, int playerX, int playerY) {
    // Render the map exactly as provided (no extra border), top-left at (1,1)
    int startX = 1, startY = 1;
    for (size_t row = 0; row < grid.size(); ++row) {
        MoveCursor(startX, startY + (int)row);
        cout << grid[row];
    }
    // Draw player '@' over the tile
    MoveCursor(startX + playerX, startY + playerY);
    cout << "@";
}

void DrawQuiz(const QuizQuestion& q) {
    ClearScreen();
    DrawBox(8, 4, 72, 22);
    int left = 10;
    int y = 6;

    // Word-wrap helper lambda
    auto wrap = [](const std::string &text, size_t maxWidth) {
        std::vector<std::string> lines;
        std::istringstream iss(text);
        std::string word;
        std::string current;
        while (iss >> word) {
            if (current.empty()) {
                current = word;
            } else if (current.size() + 1 + word.size() <= maxWidth) {
                current += " " + word;
            } else {
                lines.push_back(current);
                current = word;
            }
        }
        if (!current.empty()) lines.push_back(current);
        if (lines.empty()) lines.push_back("");
        return lines;
    };

    const int contentWidth = 72 - (left + 2); // inside DrawBox right edge is 72

    // Title
    DrawText(left, y++, "Meme Quiz");
    DrawText(left, y++, std::string(contentWidth, '='));

    // Question (wrapped)
    auto qWrapped = wrap(q.question, contentWidth - 4);
    if (!qWrapped.empty()) {
        DrawText(left, y, "Q: " + qWrapped[0]);
        y++;
        for (size_t i = 1; i < qWrapped.size(); ++i) {
            DrawText(left + 4, y++, qWrapped[i]);
        }
        y++;
    }

    // Options (wrapped)
    for (size_t i = 0; i < q.options.size(); ++i) {
        auto choiceLines = wrap(q.options[i], contentWidth - 6);
        DrawText(left, y++, std::to_string(i + 1) + ": " + choiceLines[0]);
        for (size_t li = 1; li < choiceLines.size(); ++li) {
            DrawText(left + 4, y++, choiceLines[li]);
        }
    }

    DrawText(left, y + 1, std::string("Select 1-") + std::to_string((int)q.options.size()) + " : ");
}

static string HpBar(int hp, int maxHp, int width = 20) {
    width = max(1, width);
    int filled = (int)((double)hp / maxHp * width + 0.5);
    string s(filled, '#');
    s += string(max(0, width - filled), '-');
    return s;
}

void DrawBattle(const BattleContext& ctx, const Player& player) {
    ClearScreen();
    DrawBox(2, 1, 78, 22);
    DrawText(4, 3, string("Boss: ") + ctx.boss.name);
    DrawText(4, 4, "HP [" + HpBar(ctx.boss.hp, ctx.boss.maxHp) + "] " + to_string(ctx.boss.hp) + "/" + to_string(ctx.boss.maxHp));

    DrawText(4, 7, "You:");
    DrawText(4, 8, "HP [" + HpBar(player.stats.hp, player.stats.maxHp) + "] " + to_string(player.stats.hp) + "/" + to_string(player.stats.maxHp));

    // Minimal boss ASCII
    vector<string> bossArt = {
        "    __====__",
        "   /  KAREN  \\",
        "  |  (>'-')  |",
        "  |  /|_|_|\\ |",
        "   \\   ^   //",
        "    '------'"
    };
    int artY = 10;
    for (size_t i = 0; i < bossArt.size(); ++i) {
        DrawText(48, artY + (int)i, bossArt[i]);
    }

    DrawText(4, 11, "Actions:");
    DrawText(6, 13, "1) Horrible Singing (-3)");
    if (ctx.finisherAvailable) {
        DrawText(6, 14, "2) ENERGY BEAM (Finish Him)");
    }
    DrawText(6, 16, "q) Run (forfeit)");

    if (!ctx.lastMessage.empty()) {
        DrawText(4, 19, ctx.lastMessage);
    }
    if (ctx.finisherAvailable) {
        DrawText(4, 20, "Finisher unlocked! Press 2 to end it.");
    }
}
