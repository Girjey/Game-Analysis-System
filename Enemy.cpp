#include "header files/Enemy.h"

Enemy::Enemy(std::string enemy_name_, double hp_,
             double physical_dmg_, double magic_dmg_,
             double armor_, double magic_resist_,
             double crit_damage_, double crit_chance_,
             double accuracy_, double evasion_,
             double stamina_, double max_stamina_,
            double stamina_cost_) :
    enemy_name(enemy_name_),
    hp(hp_),
    physical_damage(physical_dmg_),
    magic_damage(magic_dmg_),
    armor(armor_),
    magic_resist(magic_resist_),
    crit_damage(crit_damage_),
    crit_chance(crit_chance_),
    accuracy(accuracy_),
    evasion(evasion_),
    stamina(stamina_),
    max_stamina(max_stamina_),
    stamina_cost(stamina_cost_)
{}
const std::string& Enemy::getEnemyName() const {
    return enemy_name;
}

double Enemy::getHp() const {
    return hp;
}

double Enemy::getPhysicalDamage() const {
    return physical_damage;
}

double Enemy::getMagicDamage() const {
    return magic_damage;
}

double Enemy::getArmor() const {
    return armor;
}

double Enemy::getMagicResist() const {
    return magic_resist;
}

double Enemy::getCritDamage() const {
    return crit_damage;
}

double Enemy::getCritChance() const {
    return crit_chance;
}

double Enemy::getAccuracy() const {
    return accuracy;
}

double Enemy::getEvasion() const {
    return evasion;
}

double Enemy::getStamina() const {
    return stamina;
}

double Enemy::getMaxStamina() const {
    return max_stamina;
}

double Enemy::getStaminaCost() const {
    return stamina_cost;

}
