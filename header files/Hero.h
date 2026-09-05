#pragma once
#include <string>

class Hero {
private:

    std::string name;
    double base_hp = 100.0;
    double base_physical_damage = 50.0;
    double base_magic_damage = 0.0;
    double base_crit_damage_multiplier = 2.0;
    double base_crit_chance = 0.05;
    double base_defence = 75.0;
    double base_magic_resist_multiplier = 0.3;
    double base_accuracy = 100.0;
    double base_evasion = 10.0;
    double base_stamina = 100.0;
    double base_max_stamina = 100.0;

public:
    Hero() = default;

    Hero(const std::string& _name, double _base_hp, double _base_phys_dmg, double _base_magic_dmg,
         double _base_crit_damage, double _base_crit_chance,
         double _base_defence, double _base_magic_resist,
         double _base_accuracy, double _base_evasion,
         double _base_stamina, double _base_max_stamina);

    virtual ~Hero() = default;

    std::string get_name() const;

    double get_base_hp() const;
    double get_base_physical_damage() const;
    double get_base_magic_damage() const;
    double get_base_crit_damage() const;
    double get_base_crit_chance() const;
    double get_base_defence() const;
    double get_base_magic_resist() const;
    double get_base_accuracy() const;
    double get_base_evasion() const;
    double get_base_stamina() const;
    double get_base_max_stamina() const;

};