#pragma once
#include "Item.h"
#include "Hero.h"

class BattleUnit {
private:
    double current_hp = 0.0;
    double max_hp = 0.0;
    double own_physical_damage = 0.0;
    double own_magic_damage = 0.0;
    double own_defense = 0.0;
    double own_magic_resist = 0.0;
    double own_crit_chance = 0.0;
    double own_crit_damage = 0.0;

public:
    BattleUnit(const Hero& hero, const Item& item);

    virtual ~BattleUnit() = default;

    double get_battle_unit_hp() const;
    double get_battle_unit_max_hp() const;
    double get_battle_unit_physical_damage() const;
    double get_battle_unit_magic_damage() const;
    double get_battle_unit_defense() const;
    double get_battle_unit_magic_resist() const;
    double get_battle_unit_crit_chance() const;
    double get_battle_unit_crit_damage() const;
    void print_battle_unit_stats() const;
};