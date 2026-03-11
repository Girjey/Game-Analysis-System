#pragma once
#include <string>

class Item
{
private:
    std::string item_name = "";
    std::string item_type = "";
    double physical_damage = 0.0;
    double sharpness = 1.0;
    double magic_damage = 0.0;
    double magic_amplification = 1.0;
    double crit_damage = 2.0;
    double crit_chance = 0.05;
    double attack_speed = 1.0;
    double defense = 0.0;
    double magic_resist = 0.0;
    double weight = 0.0;
    double durability = 100.0;

public:
    Item(std::string name, std::string type, 
         double phys_dmg, double _sharpness,
         double _magic_dmg, double _magic_amplification,
         double _crit_dmg, double _crit_chance,
         double _atk_speed, double _defence, 
         double _magic_resist, double _weight, 
         double _durability);

    virtual ~Item() = default;

    double calculatePhysicalDamage() const;
    double calculateMagicDamage() const;
    double calculatePhysArmor() const;
    double calculateDPS() const;
    
    const std::string& getName() const;
    const std::string& getType() const;
    
    double getPhysicalDamage() const;
    double getSharpness() const;
    double getMagicDamage() const;
    double getMagicAmplification() const;
    double getCritDamage() const;
    double getCritChance() const;
    double getAttackSpeed() const;
    double getDef() const;
    double getMagicRest() const;
    double getWeight() const;
    double getDurability() const;
};
