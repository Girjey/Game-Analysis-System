#include "header files/Hero.h"

Hero::Hero(double _base_hp, double _base_phys_dmg, double _base_magic_dmg,
    double _base_crit_damage, double _base_crit_chance, 
    double _base_defence, double _base_magic_resist) :
    base_hp(_base_hp),
    base_physical_damage(_base_phys_dmg),
    base_magic_damage(_base_magic_dmg),
    base_crit_damage_multiplier(_base_crit_damage),
    base_crit_chance(_base_crit_chance),
    base_defence(_base_defence),
    base_magic_resist_multiplier(_base_magic_resist)
{}

double Hero::get_base_hp() const {
    return base_hp;
}

double Hero::get_base_physical_damage() const {
    return base_physical_damage;
}

double Hero::get_base_magic_damage() const {
    return base_magic_damage;
}

double Hero::get_base_crit_damage() const {
    return base_crit_damage_multiplier;
}

double Hero::get_base_crit_chance() const {
    return base_crit_chance;
}

double Hero::get_base_defence() const {
    return base_defence;
}

double Hero::get_base_magic_resist() const {
    return base_magic_resist_multiplier;
}

