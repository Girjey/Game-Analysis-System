#pragma once
#include <vector>
#include <string>
#include "Item.h"
#include "Enemy.h"
#include "Hero.h"

std::vector<Item> loadItems(const std::string& filepath);
std::vector<Enemy> loadEnemies(const std::string& filepath);
std::vector<Hero> loadHeroes(const std::string& filepath);
void listItems(const std::vector<Item>& items);
void listHeroes(const std::vector<Hero>& allHeroes);
void listEnemies(const std::vector<Enemy>& enemies);

/**
 * Создаёт нового героя через ввод характеристик и сохраняет в JSON-файл.
 * @param filepath Путь к JSON-файлу с героями
 * @return Вектор всех героев (включая нового)
 */
std::vector<Hero> createAndSaveHero(const std::string& filepath);
std::vector<Item> createAndSaveItem(const std::string& filepath);
std::vector<Enemy> createAndSaveEnemy(const std::string& filepath);
