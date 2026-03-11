#include "header files/Item.h"

Item::Item(std::string name, std::string type, 
           double phys_dmg, double _sharpness,
           double _magic_dmg, double _magic_amplification,
           double _crit_dmg, double _crit_chance,
           double _atk_speed, double _defence, 
           double _magic_resist, double _weight, 
           double _durability) :
    item_name(name),
    item_type(type),
    physical_damage(phys_dmg),
    sharpness(_sharpness),
    magic_damage(_magic_dmg),
    magic_amplification(_magic_amplification),
    crit_damage(_crit_dmg),
    crit_chance(_crit_chance),
    attack_speed(_atk_speed),
    defense(_defence),
    magic_resist(_magic_resist),
    weight(_weight),
    durability(_durability)
{}

double Item::calculatePhysicalDamage() const {
    return physical_damage * sharpness;
}

double Item::calculateDPS() const {
    return physical_damage * attack_speed * sharpness;
}

double Item::calculateMagicDamage() const {
    return magic_damage * magic_amplification;
}

double Item::calculatePhysArmor() const {
    double weightPenalty = 1.0 / (1.0 + weight * 0.1);
    double effectiveArmor = defense * weightPenalty;
    double durabilityPenalty = (durability < 25) ? (durability / 25) : 1.0;
    return effectiveArmor * durabilityPenalty;
}

const std::string& Item::getName() const {
    return item_name;
}

const std::string& Item::getType() const {
    return item_type;
}

double Item::getPhysicalDamage() const {
    return physical_damage;
}

double Item::getSharpness() const {
    return sharpness;
}

double Item::getMagicDamage() const {
    return magic_damage;
}

double Item::getMagicAmplification() const {
    return magic_amplification;
}

double Item::getCritDamage() const {
    return crit_damage;
}

double Item::getCritChance() const {
    return crit_chance;
}

double Item::getAttackSpeed() const {
    return attack_speed;
}

double Item::getDef() const {
    return defense;
}

double Item::getMagicRest() const {
    return magic_resist;
}

double Item::getWeight() const {
    return weight;
}

double Item::getDurability() const {
    return durability;
}


