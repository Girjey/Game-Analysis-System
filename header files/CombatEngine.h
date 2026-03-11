#pragma once
#include "BattleUnit.h"
#include "Enemy.h"
#include <random>

class CombatEngine {
private:
    // данные о враге
    std::string enemy_name;

    bool isHero_is_battle_unit_turn;

    mutable std::mt19937 rng;

    double enemy_hp;
    double enemy_armor;
    double enemy_magic_resist;

    double enemy_physical_damage;
    double enemy_magic_damage;

    double enemy_crit_damage;
    double enemy_chance_crit_damage;

    // данные о боевой еденице
    double battle_unit_hp;
    double battle_unit_armor;
    double battle_unit_magic_resist;

    double battle_unit_physical_damage;
    double battle_unit_magic_damage;

    double battle_unit_crit_damage;
    double battle_unit_chance_crit_damage;

public :
    CombatEngine(const BattleUnit& battle_unit, Enemy& enemy);

    virtual ~CombatEngine() = default;
    //геттеры врага
    const std::string& get_enemy_name() const;
    double get_enemy_hp() const;
    double get_enemy_armor() const;
    double get_enemy_magic_resist() const;
    double get_enemy_physical_damage() const;
    double get_enemy_magic_damage() const;
    double get_enemy_crit_damage() const;
    double get_enemy_chance_crit_damage() const;
    // геттеры боевой еденицы
    double get_battle_unit_hp() const;
    double get_battle_unit_armor() const;
    double get_battle_unit_magic_resist() const;
    double get_battle_unit_physical_damage() const;
    double get_battle_unit_magic_damage() const;
    double get_battle_unit_crit_damage() const;
    double get_battle_unit_chance_crit_damage() const;

    bool coinFlip() const;

    double calculateArmorCoefficient(double armor);
    double calculateMagicResistCoefficient(double magic_resist);

    double calculatePhysicalDamage(double physicalDamage, double critDamage, double critChanace, double defenceCoefficient);
    double calculateMagicDamage(double maficDamage, double critDamage, double critChance, double magicResistCoefficient);
    void damageEnemy();
    void damageBattleUnit();
};