#include "Save.h"
#include <fstream>
#include <sstream>

static void WriteLine(std::ofstream& out, const std::string& k, const std::string& v) {
    out << k << ":" << v << "\n";
}

bool SaveGame(const std::string& path, const Player& player, const Map& map) {
    std::ofstream out(path);
    if (!out.is_open()) return false;

    // Player
    WriteLine(out, "name", player.name);
    WriteLine(out, "memeType", player.memeType);
    WriteLine(out, "level", std::to_string(player.level));
    WriteLine(out, "xp", std::to_string(player.xp));
    WriteLine(out, "hp", std::to_string(player.stats.hp));
    WriteLine(out, "maxHp", std::to_string(player.stats.maxHp));
    WriteLine(out, "clout", std::to_string(player.stats.clout));
    WriteLine(out, "logic", std::to_string(player.stats.logic));
    WriteLine(out, "sanity", std::to_string(player.stats.sanity));
    WriteLine(out, "strength", std::to_string(player.stats.strength));
    WriteLine(out, "memePower", std::to_string(player.stats.memePower));

    // Map dims and player pos
    WriteLine(out, "mapW", std::to_string(map.width));
    WriteLine(out, "mapH", std::to_string(map.height));
    WriteLine(out, "px", std::to_string(map.playerX));
    WriteLine(out, "py", std::to_string(map.playerY));

    // Grid
    out << "grid\n";
    for (const auto& row : map.grid) out << row << "\n";
    out << "endgrid\n";

    return true;
}

static std::string ReadValue(const std::string& line) {
    auto pos = line.find(':');
    if (pos == std::string::npos) return "";
    return line.substr(pos + 1);
}

bool LoadGame(const std::string& path, Player& player, Map& map) {
    std::ifstream in(path);
    if (!in.is_open()) return false;

    std::string line;
    while (std::getline(in, line)) {
        if (line.rfind("name:", 0) == 0) player.name = ReadValue(line);
        else if (line.rfind("memeType:", 0) == 0) player.memeType = ReadValue(line);
        else if (line.rfind("level:", 0) == 0) player.level = std::stoi(ReadValue(line));
        else if (line.rfind("xp:", 0) == 0) player.xp = std::stoi(ReadValue(line));
        else if (line.rfind("hp:", 0) == 0) player.stats.hp = std::stoi(ReadValue(line));
        else if (line.rfind("maxHp:", 0) == 0) player.stats.maxHp = std::stoi(ReadValue(line));
        else if (line.rfind("clout:", 0) == 0) player.stats.clout = std::stoi(ReadValue(line));
        else if (line.rfind("logic:", 0) == 0) player.stats.logic = std::stoi(ReadValue(line));
        else if (line.rfind("sanity:", 0) == 0) player.stats.sanity = std::stoi(ReadValue(line));
        else if (line.rfind("strength:", 0) == 0) player.stats.strength = std::stoi(ReadValue(line));
        else if (line.rfind("memePower:", 0) == 0) player.stats.memePower = std::stoi(ReadValue(line));
        else if (line == "grid") break;
    }

    // Read grid
    map.grid.clear();
    while (std::getline(in, line)) {
        if (line == "endgrid") break;
        map.grid.push_back(line);
    }
    map.height = (int)map.grid.size();
    map.width = map.height ? (int)map.grid[0].size() : 0;

    // After grid, we need px/py; rewind file is complex; easier to reopen and parse px/py earlier
    // Simpler approach: reopen and parse again for px/py
    in.close();
    std::ifstream in2(path);
    if (!in2.is_open()) return false;
    while (std::getline(in2, line)) {
        if (line.rfind("px:", 0) == 0) map.playerX = std::stoi(ReadValue(line));
        else if (line.rfind("py:", 0) == 0) map.playerY = std::stoi(ReadValue(line));
    }

    return true;
}
