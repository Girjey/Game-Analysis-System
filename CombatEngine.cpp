#include "header files/CombatEngine.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <iomanip>

CombatEngine::CombatEngine(const BattleUnit &battle_unit, Enemy &enemy)
    : enemy_name(enemy.getEnemyName()),
    enemy_hp(enemy.getHp()),
    enemy_max_hp(enemy.getHp()),
    enemy_armor(enemy.getArmor()),
    enemy_magic_resist(enemy.getMagicResist()),
    enemy_physical_damage(enemy.getPhysicalDamage()),
    enemy_magic_damage(enemy.getMagicDamage()),
    enemy_crit_damage(enemy.getCritDamage()),
    enemy_chance_crit_damage(enemy.getCritChance()),
    enemy_accuracy(enemy.getAccuracy()),
    enemy_evasion(enemy.getEvasion()),
    enemy_stamina(enemy.getStamina()),
    enemy_max_stamina(enemy.getMaxStamina()),
    enemy_stamina_cost(enemy.getStaminaCost()),  // По умолчанию для врагов

    battle_unit_hp(battle_unit.get_battle_unit_hp()),
    battle_unit_max_hp(battle_unit.get_battle_unit_hp()),
    battle_unit_armor(battle_unit.get_battle_unit_defense()),
    battle_unit_magic_resist(battle_unit.get_battle_unit_magic_resist()),
    battle_unit_physical_damage(battle_unit.get_battle_unit_physical_damage()),
    battle_unit_magic_damage(battle_unit.get_battle_unit_magic_damage()),
    battle_unit_crit_damage(battle_unit.get_battle_unit_crit_damage()),
    battle_unit_chance_crit_damage(battle_unit.get_battle_unit_crit_chance()),
    battle_unit_accuracy(battle_unit.get_battle_unit_accuracy()),
    battle_unit_evasion(battle_unit.get_battle_unit_evasion()),
    battle_unit_stamina(battle_unit.get_battle_unit_current_stamina()),
    battle_unit_max_stamina(battle_unit.get_battle_unit_max_stamina()),
    battle_unit_stamina_cost(battle_unit.get_battle_unit_stamina_cost()),

    rng(std::random_device{}()),
    
    // Инициализация счётчиков статистики
    battle_unit_hits(0),
    battle_unit_misses(0),
    enemy_hits(0),
    enemy_misses(0),
    battle_unit_fatigue_turns(0),
    enemy_fatigue_turns(0)
{}

// геттеры врага
const std::string &CombatEngine::get_enemy_name() const {
    return enemy_name;
}

double CombatEngine::get_enemy_hp() const {
    return enemy_hp;
}

double CombatEngine::get_enemy_armor() const  {
    return enemy_armor;
}

double CombatEngine::get_enemy_magic_resist() const {
    return enemy_magic_resist;
}

double CombatEngine::get_enemy_physical_damage() const {
    return enemy_physical_damage;
}

double CombatEngine::get_enemy_magic_damage() const {
    return enemy_magic_damage;
}

double CombatEngine::get_enemy_crit_damage() const {
    return enemy_crit_damage;
}

double CombatEngine::get_enemy_chance_crit_damage() const {
    return enemy_chance_crit_damage;
}

double CombatEngine::get_enemy_accuracy() const {
    return enemy_accuracy;
}

double CombatEngine::get_enemy_evasion() const {
    return enemy_evasion;
}

double CombatEngine::get_enemy_stamina() const {
    return enemy_stamina;
}

double CombatEngine::get_enemy_max_stamina() const {
    return enemy_max_stamina;
}

// геттеры боевой еденицы
double CombatEngine::get_battle_unit_hp() const {
    return battle_unit_hp;
}

double CombatEngine::get_battle_unit_armor() const {
    return battle_unit_armor;
}

double CombatEngine::get_battle_unit_magic_resist() const {
    return battle_unit_magic_resist;
}

double CombatEngine::get_battle_unit_physical_damage() const {
    return battle_unit_physical_damage;
}

double CombatEngine::get_battle_unit_magic_damage() const {
    return battle_unit_magic_damage;
}

double CombatEngine::get_battle_unit_crit_damage() const {
    return battle_unit_crit_damage;
}

double CombatEngine::get_battle_unit_chance_crit_damage() const {
    return battle_unit_chance_crit_damage;
}

double CombatEngine::get_battle_unit_accuracy() const {
    return battle_unit_accuracy;
}

double CombatEngine::get_battle_unit_evasion() const {
    return battle_unit_evasion;
}

double CombatEngine::get_battle_unit_stamina() const {
    return battle_unit_stamina;
}

double CombatEngine::get_battle_unit_max_stamina() const {
    return battle_unit_max_stamina;
}

double CombatEngine::get_battle_unit_stamina_cost() const {
    return battle_unit_stamina_cost;
}

double CombatEngine::get_enemy_stamina_cost() const {
    return enemy_stamina_cost;
}

bool CombatEngine::coinFlip() const {
    std::uniform_int_distribution<int> dist(0, 1);
    return dist(rng) == 1;
}

bool CombatEngine::checkHit(double attackerAcc, double defenderEva) const {
    if (defenderEva <= 0) {
        return true; // если уклонение врага меньше либо 0 то удар пройдет
    }

    double hitChance = attackerAcc / (attackerAcc + defenderEva); // шанс удара по врагу - если у обоих будет по 100% то точность попадания 0.5
    
    std::uniform_real_distribution<double> dist(0.0, 1.0); // рандомайзер
    return dist(rng) <= hitChance; // если посчитанный шанс будет больше значения рандомайзера то удар пройдет
}

double CombatEngine::calculateArmorCoefficient(double armor) {
    return 100.0 / (100.0 + armor);
}

double CombatEngine::calculateMagicResistCoefficient(double magicResist) {
    if (magicResist <= 1.0) {
        return std::max(0.1, 1.0 - magicResist);
    }
    return 0.1;
}

double CombatEngine::calculateFatigueMultiplier(double currentStamina, double maxStamina) { // расчет мультипликатора усталости
    if (maxStamina <= 0) return 1.0;
    
    double staminaPercent = currentStamina / maxStamina; // если текущая стамина будет меньше 20 процентов от макс то штраф усталости
        return 0.3 + 0.7 * staminaPercent;
}

double CombatEngine::calculatePhysicalDamage(double physicalDamage, double critDamage,
                                              double critChance, double defenceCoefficient,
                                              double fatigueMultiplier) {
    std::uniform_real_distribution<double> critDist(0.0, 1.0);

    double critMultiplier = (critDist(rng) <= critChance) ? critDamage : 1.0; // если рандом меньше крит шанса то крит мульт присвамивается значение крит урона то есть 2 например


    double finalDamage = (physicalDamage * critMultiplier * fatigueMultiplier) * defenceCoefficient;
    return finalDamage;
}

double CombatEngine::calculateMagicDamage(double magicDamage, double critDamage,
                                           double critChance, double magicResistCoefficient,
                                           double fatigueMultiplier) {
    // Проверка на критический удар
    std::uniform_real_distribution<double> critDist(0.0, 1.0);
    double critMultiplier = (critDist(rng) <= critChance) ? critDamage : 1.0;

    // Формула: (Маг_Атака * Крит_Множитель * Штраф_усталости) * (100 / (100 + Маг_Резист))
    double finalDamage = (magicDamage * critMultiplier * fatigueMultiplier) * magicResistCoefficient;
    return finalDamage;
}

void CombatEngine::damageEnemy(double damage) {
    enemy_hp = std::max(0.0, enemy_hp - damage);
}

void CombatEngine::damageBattleUnit(double damage) {
    battle_unit_hp = std::max(0.0, battle_unit_hp - damage);
}

void CombatEngine::spendStamina(bool isBattleUnit, double cost) {
    if (isBattleUnit) {
        battle_unit_stamina = std::max(0.0, battle_unit_stamina - cost);
    } else {
        enemy_stamina = std::max(0.0, enemy_stamina - cost);
    }
}

void CombatEngine::regenStamina(bool isBattleUnit) {
    if (isBattleUnit) {
        battle_unit_stamina = std::min(battle_unit_max_stamina, battle_unit_stamina + STAMINA_REGEN_PER_TURN);
    } else {
        enemy_stamina = std::min(enemy_max_stamina, enemy_stamina + STAMINA_REGEN_PER_TURN);
    }
}

CombatEngine::BattleAnalysis CombatEngine::runNewSimulation(int sim_turns){

    BattleAnalysis analysis;
    analysis.simCount = sim_turns;

    std::cout << "Бой боевой единицы против " << enemy_name << "\n\n";
    std::cout << std::left;
    std::cout << std::setw(25) << enemy_name << " | " << "Боевая единица" << "\n";
    std::cout << std::string(50, '-') << "\n";
    std::cout << std::setw(25) << ("HP: " + std::to_string(static_cast<int>(enemy_hp))) << " |  " << "HP: " << battle_unit_hp << "\n";
    std::cout << std::setw(25) << ("Броня: " + std::to_string(static_cast<int>(enemy_armor))) << " |    " << "Броня: " << battle_unit_armor << "\n";
    std::cout << std::setw(25) << ("Маг. сопр.: " + std::to_string(static_cast<int>(enemy_magic_resist))) << " |    " << "Маг. сопр.: " << battle_unit_magic_resist << "\n";
    std::cout << std::setw(25) << ("Физ. урон: " + std::to_string(static_cast<int>(enemy_physical_damage))) << " |  " << "Физ. урон: " << battle_unit_physical_damage << "\n";
    std::cout << std::setw(25) << ("Маг. урон: " + std::to_string(static_cast<int>(enemy_magic_damage))) << " | " << "Маг. урон: " << battle_unit_magic_damage << "\n";
    std::cout << std::setw(25) << ("Крит. урон: " + std::to_string(static_cast<int>(enemy_crit_damage))) << " | " << "Крит. урон: " << battle_unit_crit_damage << "\n";
    std::cout << std::setw(25) << ("Шанс крита: " + std::to_string(static_cast<int>(enemy_chance_crit_damage))) << " |  " << "Шанс крита: " << battle_unit_chance_crit_damage << "\n";
    std::cout << std::setw(25) << ("Точность: " + std::to_string(static_cast<int>(enemy_accuracy))) << " |  " << "Точность: " << battle_unit_accuracy << "\n";
    std::cout << std::setw(25) << ("Уклонение: " + std::to_string(static_cast<int>(enemy_evasion))) << " |  " << "Уклонение: " << battle_unit_evasion << "\n";
    std::cout << std::setw(25) << ("Выносливость: " + std::to_string(static_cast<int>(enemy_stamina)) + "/" + std::to_string(static_cast<int>(enemy_max_stamina))) << " |   " << "Выносливость: " << battle_unit_stamina << "/" << battle_unit_max_stamina << "\n";



    std::cout << "Начало боевой симуляции: " << std::endl;

    for (size_t i = 0; i < sim_turns; i++) {
        analysis.history.clear();
        // Сбрасываем HP и стамину для нового боя
        enemy_hp = enemy_max_hp;
        enemy_stamina = enemy_max_stamina;
        battle_unit_hp = battle_unit_max_hp;
        battle_unit_stamina = battle_unit_max_stamina;

        bool heroFatiguedBefore = false;
        bool enemyFatiguedBefore = false;
        int turnCounter = 1;
        std::vector<TurnLog> currentBattleHistory;

        // Определяем, кто ходит первым в этом бою
        bool isHeroTurn = coinFlip();

        while (battle_unit_hp > 0 && enemy_hp > 0) {
            TurnLog log;
            log.turn = turnCounter;
            log.attacker = isHeroTurn ? "Герой" : enemy_name;
            log.isCrit = false;
            log.fatigueStarted = false;

            double armorCoef = isHeroTurn ? calculateArmorCoefficient(enemy_armor) : calculateArmorCoefficient(battle_unit_armor);
            double curStamina = isHeroTurn ? battle_unit_stamina : enemy_stamina;
            double maxStamina = isHeroTurn ? battle_unit_max_stamina : enemy_max_stamina;

            double fMult = calculateFatigueMultiplier(curStamina, maxStamina);
            log.isFatigued = (fMult < 1.0);

            if (log.isFatigued && !(isHeroTurn ? heroFatiguedBefore : enemyFatiguedBefore)) {
                log.fatigueStarted = true;
                if (isHeroTurn) heroFatiguedBefore = true; else enemyFatiguedBefore = true;
            }

            // РАСЧЕТ УДАРА
            double acc = isHeroTurn ? battle_unit_accuracy : enemy_accuracy;
            double eva = isHeroTurn ? enemy_evasion : battle_unit_evasion;

            if (checkHit(acc * fMult, eva)) {
                double baseDmg = isHeroTurn ? battle_unit_physical_damage : enemy_physical_damage;
                double cDmg = isHeroTurn ? battle_unit_crit_damage : enemy_crit_damage;
                double cChance = isHeroTurn ? battle_unit_chance_crit_damage : enemy_chance_crit_damage;


                log.isCrit = (std::uniform_real_distribution<double>(0, 1)(rng) <= cChance);
                double rawPhysDmg = baseDmg * (log.isCrit ? cDmg : 1.0) * fMult;

                double physFinal = rawPhysDmg * armorCoef;
                double physBlocked = rawPhysDmg - physFinal;

                double baseMagic = isHeroTurn ? battle_unit_magic_damage : enemy_magic_damage;
                double magicResistCoef = isHeroTurn
                ? calculateMagicResistCoefficient(enemy_magic_resist)
                : calculateMagicResistCoefficient(battle_unit_magic_resist);

                double rawMagicDamage = baseMagic * (log.isCrit ? cDmg : 1.0) * fMult;
                double magicFinal = rawMagicDamage * magicResistCoef;
                double magicBlocked = rawMagicDamage - magicFinal;

                log.damageDealt = physFinal + magicFinal;
                log.armorBlocked = physBlocked + magicBlocked;

                if (isHeroTurn) {
                    damageEnemy(log.damageDealt);
                    spendStamina(true, battle_unit_stamina_cost);
                    log.targetHp = enemy_hp;
                } else {
                    damageBattleUnit(log.damageDealt);
                    spendStamina(false, enemy_stamina_cost);
                    log.targetHp = battle_unit_hp;
                }
            } else {
                log.damageDealt = 0;
                log.targetHp = isHeroTurn ? enemy_hp : battle_unit_hp;
            }

            // Сохраняем историю для всех боёв
            analysis.history.push_back(log);
            currentBattleHistory.push_back(log);

            regenStamina(isHeroTurn);
            regenStamina(!isHeroTurn);

            // Передаём ход другому бойцу
            isHeroTurn = !isHeroTurn;
            turnCounter++;  // Увеличиваем счётчик ходов
        }

        // Сохраняем историю текущего боя в отдельный вектор
        analysis.allBattlesHistory.push_back(currentBattleHistory);
        
        // Сохраняем результат боя
        bool won = (enemy_hp <= 0);
        analysis.battleResults.push_back(won);
        
        if (won) analysis.totalWins++;
        else analysis.totalLosses++;
    }

    return analysis;
}

void CombatEngine::printFullLog(const BattleAnalysis& analysis) {
    std::cout << "\n=== ЛОГ ПЕРВОГО БОЯ ===" << std::endl;

    for (const auto& log : analysis.history) {
        std::cout << "[Ход " << log.turn << "] " << log.attacker;

        if (log.damageDealt > 0) {
            std::cout << " наносит " << log.damageDealt << " ед.";
            if (log.isCrit) std::cout << " (КРИТ!)";
            std::cout << " | Броня: " << log.armorBlocked;
            std::cout << " | HP: " << log.targetHp;
        } else {
            std::cout << " ПРОМАХ";
        }
        std::cout << std::endl;
    }

    std::cout << "\nИТОГ: Побед " << analysis.totalWins << " из " << analysis.simCount << std::endl;
}

void CombatEngine::printAllBattlesLog(const BattleAnalysis& analysis) {
    std::cout << "\n========== ПОЛНАЯ СТАТИСТИКА ВСЕХ БОЁВ ==========" << std::endl;
    
    for (size_t i = 0; i < analysis.allBattlesHistory.size(); i++) {
        const auto& battle = analysis.allBattlesHistory[i];
        bool won = analysis.battleResults[i];
        
        std::cout << "\n--- БОЙ #" << (i + 1) << " ---" << std::endl;
        
        for (const auto& log : battle) {
            std::cout << "  Ход " << log.turn << ": " << log.attacker;
            
            if (log.damageDealt > 0) {
                std::cout << " -> " << log.damageDealt << " ед.";
                if (log.isCrit) std::cout << " [КРИТ]";
                if (log.isFatigued) std::cout << " [УСТАЛОСТЬ]";
                std::cout << " (Броня: " << log.armorBlocked << ")";
                std::cout << " | HP цели: " << log.targetHp;
            } else {
                std::cout << " -> ПРОМАХ";
            }
            std::cout << std::endl;
        }
        
        std::cout << "  Результат: " << (won ? "ПОБЕДА" : "ПОРАЖЕНИЕ") << std::endl;
    }
    
    std::cout << "\n========== ИТОГ ==========" << std::endl;
    std::cout << "Всего боёв: " << analysis.simCount << std::endl;
    std::cout << "Побед: " << analysis.totalWins << " | Поражений: " << analysis.totalLosses << std::endl;
    double winRate = (double)analysis.totalWins / analysis.simCount * 100.0;
    std::cout << "Winrate: " << winRate << "%" << std::endl;
    std::cout << "==========================" << std::endl;
}




