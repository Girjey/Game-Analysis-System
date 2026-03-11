#pragma once
#include <vector>
#include <string>
#include "Item.h"
#include "Enemy.h"

std::vector<Item> loadItems(const std::string& filepath);
std::vector<Enemy> loadEnemies(const std::string& filepath);