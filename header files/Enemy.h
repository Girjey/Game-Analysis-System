#pragma once
#include <string>

class Enemy {
private:
    std::string enemy_name = "";
    double hp = 100;
    double physical_damage = 50;
    double magic_damage = 0;
    double armor = 50;
    double magic_resist = 0.25;
    double crit_damage = 2.0;
    double crit_chance = 0.0;
    double accuracy = 100.0;
    double evasion = 0.0;
    double stamina = 100.0;
    double max_stamina = 100.0;
    double stamina_cost = 5.0;

public:
    Enemy(std::string enemy_name_, double hp_,
          double physical_dmg_, double magic_dmg_,
          double armor_, double magic_resist_,
          double crit_damage_, double crit_chance_,
          double accuracy_, double evasion_,
          double stamina_, double max_stamina_,
          double stamina_cost_);

    virtual ~Enemy() = default;

    const std::string& getEnemyName() const;
    double getHp() const;
    double getPhysicalDamage() const;
    double getMagicDamage() const;
    double getArmor() const;
    double getMagicResist() const;
    double getCritDamage() const;
    double getCritChance() const;
    double getAccuracy() const;
    double getEvasion() const;
    double getStamina() const;
    double getMaxStamina() const;
    double getStaminaCost() const;
};