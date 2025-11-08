#ifndef SAVE_H
#define SAVE_H

#include <string>
#include "GameData.h"
#include "Map.h"

bool SaveGame(const std::string& path, const Player& player, const Map& map);
bool LoadGame(const std::string& path, Player& player, Map& map);

#endif // SAVE_H
