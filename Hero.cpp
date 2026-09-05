#include "header files/Hero.h"

Hero::Hero(const std::string& _name, double _base_hp, double _base_phys_dmg, double _base_magic_dmg,
    double _base_crit_damage, double _base_crit_chance,
    double _base_defence, double _base_magic_resist,
    double _base_accuracy, double _base_evasion,
    double _base_stamina, double _base_max_stamina) :
    name(_name),
    base_hp(_base_hp),
    base_physical_damage(_base_phys_dmg),
    base_magic_damage(_base_magic_dmg),
    base_crit_damage_multiplier(_base_crit_damage),
    base_crit_chance(_base_crit_chance),
    base_defence(_base_defence),
    base_magic_resist_multiplier(_base_magic_resist),
    base_accuracy(_base_accuracy),
    base_evasion(_base_evasion),
    base_stamina(_base_stamina),
    base_max_stamina(_base_max_stamina)
{}

std::string Hero::get_name() const {
    return name;
}

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

double Hero::get_base_accuracy() const {
    return base_accuracy;
}

double Hero::get_base_evasion() const {
    return base_evasion;
}

double Hero::get_base_stamina() const {
    return base_stamina;
}

double Hero::get_base_max_stamina() const {
    return base_max_stamina;
}

