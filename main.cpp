#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

#include "header files/Item.h"
#include "header files/Enemy.h"
#include "header files/DataLoader.h"
#include "header files/Hero.h"
#include "header files/BattleUnit.h"
#include "header files/CombatEngine.h"


int main()
{
    SetConsoleOutputCP(CP_UTF8);

    std::vector<Item> items = loadItems("Items.json");
    std::vector<Enemy> enemies = loadEnemies("enemies.json");

    Hero hero;

    const Item& item = items[0];

    BattleUnit battle_unit(hero, item);

    battle_unit.print_battle_unit_stats();
    
    std::cout << "\n=== Предмет ===" << std::endl;
    std::cout << "Название: " << item.getName() << std::endl;
    std::cout << "Физ. урон: " << item.getPhysicalDamage() << std::endl;
    std::cout << "Маг. урон: " << item.getMagicDamage() << std::endl;
    std::cout << "Расчётный физ. урон: " << item.calculatePhysicalDamage() << std::endl;
    std::cout << "Расчётный маг. урон: " << item.calculateMagicDamage() << std::endl;
    std::cout << "Броня: " << item.calculatePhysArmor() << std::endl;

    std::cout << "\n=== Враги ===" << std::endl;
    for (const auto& enemy : enemies) {
        std::cout << enemy.getEnemyName() << ": HP=" << enemy.getHp() 
                  << ", Физ. урон=" << enemy.getPhysicalDamage()
                  << ", Маг. урон=" << enemy.getMagicDamage()
                  << ", Крит. шанс=" << enemy.getCritChance() * 100 << "%"
                  << std::endl;
    }
    CombatEngine engine1(battle_unit, enemies[0]);
    std::cout << engine1.get_enemy_name() << std::endl;
    // подброс монеты работает

    return 0;
}
