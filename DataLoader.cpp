#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include "header files/Item.h"
#include "header files/Enemy.h"

using json = nlohmann::json;

std::vector<Item> loadItems(const std::string& filepath) {
    std::vector<Item> items;

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << filepath << std::endl;
        return items;
    }

    json data = json::parse(file, nullptr, false);
    if (data.is_discarded()) {
        std::cerr << "Ошибка парсинга JSON" << std::endl;
        return items;
    }

    for (const auto& item_json : data["items"]) {
        Item item(
            item_json.value("name", ""),
            item_json.value("type", ""),
            item_json.value("physical_damage", 0.0),
            item_json.value("sharpness", 1.0),
            item_json.value("magic_damage", 0.0),
            item_json.value("magic_amplification", 1.0),
            item_json.value("crit_damage", 2.0),
            item_json.value("crit_chance", 0.05),
            item_json.value("attack_speed", 1.0),
            item_json.value("defense", 0.0),
            item_json.value("magic_resist", 0.0),
            item_json.value("weight", 0.0),
            item_json.value("durability", 100.0)
        );
        items.push_back(item);
    }

    return items;
}

std::vector<Enemy> loadEnemies(const std::string& filepath) {
    std::vector<Enemy> enemies;

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << filepath << std::endl;
        return enemies;
    }

    json data = json::parse(file, nullptr, false);
    if (data.is_discarded()) {
        std::cerr << "Ошибка парсинга JSON" << std::endl;
        return enemies;
    }

    for (const auto& enemy_json : data["enemies"]) {
        Enemy enemy(
            enemy_json.value("name", ""),
            enemy_json.value("hp", 100.0),
            enemy_json.value("physical_damage", 50.0),
            enemy_json.value("magic_damage", 0.0),
            enemy_json.value("armor", 0.0),
            enemy_json.value("magic_resist", 0.0),
            enemy_json.value("crit_damage", 2.0),
            enemy_json.value("crit_chance", 0.0)
        );
        enemies.push_back(enemy);
    }

    return enemies;
}
