#include "header files/BattleUnit.h"
#include <iostream>

BattleUnit::BattleUnit(const Hero& hero, const Item& item)
    : current_hp(hero.get_base_hp()),
      max_hp(hero.get_base_hp()),
      own_physical_damage(hero.get_base_physical_damage() + item.getPhysicalDamage()),
      own_magic_damage(hero.get_base_magic_damage() + item.getMagicDamage()),
      own_defense(hero.get_base_defence() + item.calculatePhysArmor()),
      own_magic_resist(hero.get_base_magic_resist() + item.getMagicRest()),
      own_crit_chance(hero.get_base_crit_chance() + item.getCritChance()),
      own_crit_damage(hero.get_base_crit_damage() + item.getCritDamage()),
      own_accuracy(hero.get_base_accuracy()),
      own_evasion(hero.get_base_evasion()),
      current_stamina(hero.get_base_stamina()),
      max_stamina(hero.get_base_max_stamina()),
      stamina_cost_per_hit(item.getStaminaCost())
{}

double BattleUnit::get_battle_unit_hp() const {
    return current_hp;
}

double BattleUnit::get_battle_unit_max_hp() const {
    return max_hp;
}

double BattleUnit::get_battle_unit_physical_damage() const {
    return own_physical_damage;
}

double BattleUnit::get_battle_unit_magic_damage() const {
    return own_magic_damage;
}

double BattleUnit::get_battle_unit_defense() const {
    return own_defense;
}

double BattleUnit::get_battle_unit_magic_resist() const {
    return own_magic_resist;
}

double BattleUnit::get_battle_unit_crit_chance() const {
    return own_crit_chance;
}

double BattleUnit::get_battle_unit_crit_damage() const {
    return own_crit_damage;
}

double BattleUnit::get_battle_unit_accuracy() const {
    return own_accuracy;
}

double BattleUnit::get_battle_unit_evasion() const {
    return own_evasion;
}

double BattleUnit::get_battle_unit_current_stamina() const {
    return current_stamina;
}

double BattleUnit::get_battle_unit_max_stamina() const {
    return max_stamina;
}

double BattleUnit::get_battle_unit_stamina_cost() const {
    return stamina_cost_per_hit;
}

void BattleUnit::print_battle_unit_stats() const {
    std::cout << "=== Характеристики боевой единицы ===" << std::endl;
    std::cout << "HP: " << current_hp << " / " << max_hp << std::endl;
    std::cout << "Физ. урон: " << own_physical_damage << std::endl;
    std::cout << "Маг. урон: " << own_magic_damage << std::endl;
    std::cout << "Защита: " << own_defense << std::endl;
    std::cout << "Маг. сопротивление: " << own_magic_resist << std::endl;
    std::cout << "Шанс крита: " << own_crit_chance * 100 << "%" << std::endl;
    std::cout << "Крит. урон: " << own_crit_damage << std::endl;
    std::cout << "Точность: " << own_accuracy << std::endl;
    std::cout << "Уклонение: " << own_evasion << std::endl;
    std::cout << "Стамина: " << current_stamina << " / " << max_stamina << std::endl;
    std::cout << "Стоимость атаки: " << stamina_cost_per_hit << std::endl;
}
