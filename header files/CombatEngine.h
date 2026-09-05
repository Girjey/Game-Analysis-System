#pragma once
#include "BattleUnit.h"
#include "Enemy.h"
#include <random>
#include <string>
#include <vector>

// Константы баланса стамины
const double STAMINA_COST_PER_HIT = 10.0;
const double STAMINA_REGEN_PER_TURN = 4.0;
const double FATIGUE_THRESHOLD = 0.2; // 20%

class CombatEngine {
public:
    struct TurnLog {
        int turn;
        std::string attacker;
        double damageDealt;
        double armorBlocked;
        bool isCrit;
        bool isFatigued;
        bool fatigueStarted;
        double targetHp;
    };

    struct BattleAnalysis {
        std::vector<std::vector<TurnLog>> allBattlesHistory;  // История всех боёв
        std::vector<TurnLog> history;  // История первого боя (для совместимости)
        std::vector<bool> battleResults;  // Результаты боёв: true = победа, false = поражение
        int totalWins = 0;
        int totalLosses = 0;
        int simCount = 0;
    };

private:
    // данные о враге
    std::string enemy_name;

    bool isHero_is_battle_unit_turn;

    mutable std::mt19937 rng;

    double enemy_hp;
    double enemy_max_hp;
    double enemy_armor;
    double enemy_magic_resist;

    double enemy_physical_damage;
    double enemy_magic_damage;

    double enemy_crit_damage;
    double enemy_chance_crit_damage;
    
    // Новые метрики врага
    double enemy_accuracy;
    double enemy_evasion;
    double enemy_stamina;
    double enemy_max_stamina;
    double enemy_stamina_cost;

    // данные о боевой еденице
    double battle_unit_hp;
    double battle_unit_max_hp;
    double battle_unit_armor;
    double battle_unit_magic_resist;

    double battle_unit_physical_damage;
    double battle_unit_magic_damage;

    double battle_unit_crit_damage;
    double battle_unit_chance_crit_damage;
    
    // Новые метрики боевой единицы
    double battle_unit_accuracy;
    double battle_unit_evasion;
    double battle_unit_stamina;
    double battle_unit_max_stamina;
    double battle_unit_stamina_cost;
    
    // Статистика боя
    int battle_unit_hits;
    int battle_unit_misses;
    int enemy_hits;
    int enemy_misses;
    int battle_unit_fatigue_turns;
    int enemy_fatigue_turns;

public:
    CombatEngine(const BattleUnit& battle_unit, Enemy& enemy);

    virtual ~CombatEngine() = default;
    //геттеры врага
    const std::string& get_enemy_name() const;
    double get_enemy_hp() const;
    double get_enemy_armor() const;
    double get_enemy_magic_resist() const;
    double get_enemy_physical_damage() const;
    double get_enemy_magic_damage() const;
    double get_enemy_crit_damage() const;
    double get_enemy_chance_crit_damage() const;
    double get_enemy_accuracy() const;
    double get_enemy_evasion() const;
    double get_enemy_stamina() const;
    double get_enemy_max_stamina() const;
    // геттеры боевой еденицы
    double get_battle_unit_hp() const;
    double get_battle_unit_armor() const;
    double get_battle_unit_magic_resist() const;
    double get_battle_unit_physical_damage() const;
    double get_battle_unit_magic_damage() const;
    double get_battle_unit_crit_damage() const;
    double get_battle_unit_chance_crit_damage() const;
    double get_battle_unit_accuracy() const;
    double get_battle_unit_evasion() const;
    double get_battle_unit_stamina() const;
    double get_battle_unit_max_stamina() const;

    bool coinFlip() const;
    
    // Проверка попадания (точность vs уклонение)
    bool checkHit(double attackerAcc, double defenderEva) const;

    double calculateArmorCoefficient(double armor);
    double calculateMagicResistCoefficient(double magicResist);

    double calculatePhysicalDamage(double physicalDamage, double critDamage, double critChance, double defenceCoefficient, double fatigueMultiplier);
    double calculateMagicDamage(double magicDamage, double critDamage, double critChance, double magicResistCoefficient, double fatigueMultiplier);

    // Расчёт штрафа от усталости
    double calculateFatigueMultiplier(double currentStamina, double maxStamina);

    void damageEnemy(double damage);
    void damageBattleUnit(double damage);

    // Управление стаминой
    void spendStamina(bool isBattleUnit, double cost);
    void regenStamina(bool isBattleUnit);

    BattleAnalysis runNewSimulation(int sim_turns);
    void printFullLog(const BattleAnalysis& battleAnalysis);
    void printAllBattlesLog(const BattleAnalysis& analysis);

    double get_battle_unit_stamina_cost() const;
    double get_enemy_stamina_cost() const;
};