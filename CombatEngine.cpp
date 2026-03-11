#include "header files/CombatEngine.h"
#include <iostream>
#include <random>

CombatEngine::CombatEngine(const BattleUnit &battle_unit, Enemy &enemy)
    : enemy_name(enemy.getEnemyName()),
    enemy_hp(enemy.getHp()),
    enemy_armor(enemy.getArmor()),
    enemy_magic_resist(enemy.getMagicResist()),
    enemy_physical_damage(enemy.getPhysicalDamage()),
    enemy_magic_damage(enemy.getMagicDamage()),
    enemy_crit_damage(enemy.getCritDamage()),
    enemy_chance_crit_damage(enemy.getCritChance()),

    battle_unit_hp(battle_unit.get_battle_unit_hp()),
    battle_unit_armor(battle_unit.get_battle_unit_defense()),
    battle_unit_magic_resist(battle_unit.get_battle_unit_magic_resist()),
    battle_unit_physical_damage(battle_unit.get_battle_unit_physical_damage()),
    battle_unit_magic_damage(battle_unit.get_battle_unit_magic_damage()),
    battle_unit_crit_damage(battle_unit.get_battle_unit_crit_damage()),
    battle_unit_chance_crit_damage(battle_unit.get_battle_unit_crit_chance()),
    
    rng(std::random_device{}())
{}
// геттеры врага
const std::string &CombatEngine::get_enemy_name() const {
    return enemy_name;
}

double CombatEngine::get_enemy_hp() const {
    return enemy_hp;
}

double CombatEngine::get_enemy_armor() const  {
    return enemy_armor;
}

double CombatEngine::get_enemy_magic_resist() const {
    return enemy_magic_resist;
}

double CombatEngine::get_enemy_physical_damage() const {
    return enemy_magic_resist;
}

double CombatEngine::get_enemy_magic_damage() const {
    return enemy_magic_damage;
}

double CombatEngine::get_enemy_crit_damage() const {
    return enemy_crit_damage;
}

double CombatEngine::get_enemy_chance_crit_damage() const {
    return enemy_chance_crit_damage;
}
// геттеры боевой еденицы
double CombatEngine::get_battle_unit_hp() const {
    return battle_unit_hp;
}

double CombatEngine::get_battle_unit_armor() const {
    return battle_unit_armor;
}

double CombatEngine::get_battle_unit_magic_resist() const {
    return battle_unit_magic_resist;
}

double CombatEngine::get_battle_unit_physical_damage() const {
    return battle_unit_physical_damage;
}

double CombatEngine::get_battle_unit_magic_damage() const {
    return battle_unit_magic_damage;
}
double CombatEngine::get_battle_unit_crit_damage() const {
    return battle_unit_crit_damage;
}

double CombatEngine::get_battle_unit_chance_crit_damage() const {
    return battle_unit_chance_crit_damage;
}

bool CombatEngine::coinFlip() const {
    std::uniform_int_distribution<int> dist(0, 1);
    return dist(rng) == 1;
}

double CombatEngine::calculateArmorCoefficient(double armor) {
    double armorCoefficient = 100 / (100 + armor);
}

double CombatEngine::calculateMagicResistCoefficient(double magicResist) {
    double magicResistCoefficient = 1 - magicResist;
}

