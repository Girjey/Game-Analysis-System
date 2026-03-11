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

public:
    Enemy(std::string enemy_name_, double hp_,
          double physical_dmg_, double magic_dmg_, 
          double armor_, double magic_resist_, 
          double crit_damage_, double crit_chance_);

    virtual ~Enemy() = default;

    const std::string& getEnemyName() const;
    double getHp() const;
    double getPhysicalDamage() const;
    double getMagicDamage() const;
    double getArmor() const;
    double getMagicResist() const;
    double getCritDamage() const;
    double getCritChance() const;
};